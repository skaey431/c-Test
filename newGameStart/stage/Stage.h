#pragma once
#include <string>

class Stage {
private:
    std::string name;

public:
    Stage(const std::string& name);
    void describe();
};
