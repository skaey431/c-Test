#include "Game.h"
#include <iostream>
using namespace std;

Game::Game() : running(true), stage("Tutorial") {}

void Game::run() {
    cout << "게임 시작!\n";

    while (running) {
        cout << "\n명령 입력 (move/quit): ";
        string cmd;
        cin >> cmd;

        if (cmd == "move") {
            stage.describe();
        } else if (cmd == "quit") {
            running = false;
        }
    }

    cout << "게임 종료!\n";
}
