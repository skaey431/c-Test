#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

class JsonReader {
public:
    static bool readJson(const std::string& filepath, json& outJson) {
        try {
            std::ifstream file(filepath);
            if (!file.is_open()) {
                std::cerr << "파일 열기 실패: " << filepath << "\n";
                return false;
            }

            file >> outJson;
            return true;
        }
        catch (std::exception& e) {
            std::cerr << "JSON 읽기 오류: " << e.what() << "\n";
            return false;
        }
    }
};
