#pragma once
#include "Player.h"
#include "Stage.h"

class Game {
private:
    Player player;
    Stage stage;
    bool running;

public:
    Game();
    void run();
};
