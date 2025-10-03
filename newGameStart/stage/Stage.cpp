#include "Stage.h"
#include <iostream>
using namespace std;

Stage::Stage(const std::string& name) : name(name) {}

void Stage::describe() {
    cout << "당신은 " << name << "에 있습니다.\n";
}
