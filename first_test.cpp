#include <SFML/Graphics.hpp>
#include <iostream>

// 이동 컴포넌트
class MovementComponent {
public:
    void move(sf::CircleShape& shape, float dx, float dy) {
        shape.move(dx, dy);
    }
};

// 공격 컴포넌트
class AttackComponent {
public:
    void attack() {
        std::cout << "공격 실행!\n";
    }
};

// 엔티티
class Entity {
public:
    sf::CircleShape shape;
    MovementComponent* movement;
    AttackComponent* attack;

    Entity(MovementComponent* m, AttackComponent* a)
        : movement(m), attack(a)
    {
        shape.setRadius(30.f);
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(400.f, 300.f);
    }

    void performMove(float dx, float dy) {
        if (movement) movement->move(shape, dx, dy);
    }

    void performAttack() {
        if (attack) attack->attack();
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Entity Example");

    MovementComponent m;
    AttackComponent a;
    Entity player(&m, &a);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 입력 처리
        float speed = 5.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) player.performMove(0, -speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) player.performMove(0, speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) player.performMove(-speed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) player.performMove(speed, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) player.performAttack();

        // 화면 출력
        window.clear(sf::Color::Black);
        window.draw(player.shape);
        window.display();
    }
}
