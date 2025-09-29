// terminal_entity_linux.cpp
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <csignal>

#ifndef _WIN32
  #include <termios.h>
  #include <unistd.h>
  #include <sys/select.h>
#endif

// ---------- terminal raw mode (Unix) ----------
#ifndef _WIN32
static struct termios orig_termios;

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) return;
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON); // no echo, non-canonical
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int kbhit() {
    timeval tv{0, 0};
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    return select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &tv) > 0;
}

int getch_nonblock() {
    unsigned char c;
    ssize_t n = read(STDIN_FILENO, &c, 1);
    if (n == 1) return c;
    return -1;
}
#endif
// -----------------------------------------------

// Movement component
class MovementComponent {
public:
    void move(int& x, int& y, int dx, int dy, int w, int h) {
        int nx = x + dx;
        int ny = y + dy;
        if (nx >= 0 && nx < w && ny >= 0 && ny < h) {
            x = nx; y = ny;
        }
    }
};

// Attack component
class AttackComponent {
public:
    void attack() {
        // 콘솔 로그
        std::cout << ">>> 공격 실행!\n";
    }
};

// Entity
class Entity {
public:
    int x, y;
    MovementComponent* movement;
    AttackComponent* attack;
    char glyph;

    Entity(int sx, int sy, MovementComponent* m, AttackComponent* a, char g = '@')
        : x(sx), y(sy), movement(m), attack(a), glyph(g) {}

    void performMove(int dx, int dy, int w, int h) {
        if (movement) movement->move(x, y, dx, dy, w, h);
    }

    void performAttack() {
        if (attack) attack->attack();
    }
};

// 화면 지우기 (ANSI)
void clearScreen() {
    std::cout << "\x1B[2J\x1B[H";
}

// 미니맵 그리기
void drawMap(int width, int height, const std::vector<Entity*>& ents) {
    std::vector<std::string> grid(height, std::string(width, '.'));

    for (auto e : ents) {
        if (e->x >= 0 && e->x < width && e->y >= 0 && e->y < height) {
            grid[e->y][e->x] = e->glyph;
        }
    }

    for (int y = 0; y < height; ++y) {
        std::cout << grid[y] << '\n';
    }
}

// 안전 종료를 위한 전역 플래그
static bool g_running = true;

#ifndef _WIN32
void signal_handler(int signum) {
    // restore terminal
    disable_raw_mode();
    std::cout << "\n종료 신호(" << signum << ") 수신, 터미널 복구 후 종료합니다.\n";
    std::exit(0);
}
#endif

int main() {
#ifndef _WIN32
    // 시그널 처리 (Ctrl+C 등)
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    enable_raw_mode(); // non-blocking 입력 모드
#endif

    const int W = 30, H = 12;
    MovementComponent mv;
    AttackComponent at;
    Entity player(W/2, H/2, &mv, &at, '@');

    // 예시 몹 하나 (움직이지 않음)
    Entity mob(4, 3, nullptr, nullptr, 'M');

    std::vector<Entity*> ents = { &player, &mob };

    clearScreen();
    std::cout << "W/A/S/D 이동, Space 공격, Q 종료\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    const int TICK_MS = 60; // 프레임(루프) 주기

    while (g_running) {
        int c = -1;
#ifndef _WIN32
        if (kbhit()) c = getch_nonblock();
#endif

        if (c != -1) {
            // 단일 문자 입력 처리
            if (c == 'w' || c == 'W') player.performMove(0, -1, W, H);
            else if (c == 's' || c == 'S') player.performMove(0, 1, W, H);
            else if (c == 'a' || c == 'A') player.performMove(-1, 0, W, H);
            else if (c == 'd' || c == 'D') player.performMove(1, 0, W, H);
            else if (c == ' ' || c == '\r') player.performAttack();
            else if (c == 'q' || c == 'Q') { g_running = false; break; }
            // ESC / 화살표 등 복합 시퀀스는 이 버전에서 생략
        }

        // 화면 갱신
        clearScreen();
        std::cout << "W/A/S/D 이동, Space 공격, Q 종료\n\n";
        drawMap(W, H, ents);

        // 상태 출력 (좌표)
        std::cout << "\n플레이어 좌표: (" << player.x << ", " << player.y << ")\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(TICK_MS));
    }

#ifndef _WIN32
    disable_raw_mode();
#endif

    std::cout << "프로그램 종료.\n";
    return 0;
}
