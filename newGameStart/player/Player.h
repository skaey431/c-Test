#pragma once
#include <string>

class Player {
private:
    std::string name;
    int hp;
    int attackPower;

public:
    Player(const std::string& name = "Hero", int hp = 100, int attackPower = 10);

    void showStatus();
    void takeDamage(int dmg);
    int getAttackPower() const;
};
