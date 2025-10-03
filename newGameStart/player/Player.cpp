#include "Player.h"
#include <iostream>
using namespace std;

Player::Player(const std::string& name, int hp, int attackPower)
    : name(name), hp(hp), attackPower(attackPower) {}

void Player::showStatus() {
    cout << "플레이어: " << name << ", HP: " << hp << ", 공격력: " << attackPower << "\n";
}

void Player::takeDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
    cout << name << "이(가) " << dmg << " 피해를 입었습니다. 남은 HP: " << hp << "\n";
}

int Player::getAttackPower() const {
    return attackPower;
}
