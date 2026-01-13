#pragma once

#include <fstream>
#include <string>

#include "types/Task.hpp"
#include "types/Solution.hpp"
#include "types/Rock.hpp"


template <typename T>
void to_file(std::ofstream& fs, const std::vector<T>& vec) {
    for (auto& obj : vec) {
        fs << obj;
    }
}

template <typename T>
void to_file(std::string filename, const std::vector<T>& vec) {
    std::ofstream fs(filename);
    to_file(fs, vec);
}

void to_file(std::ofstream& fs, const std::string& str) {
    fs << str;
}

void to_file(std::string filename, const std::string& str) {
    std::ofstream fs(filename);
    to_file(fs, str);
}

