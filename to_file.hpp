#pragma once

#include <fstream>
#include <vector>


template <typename T>
void to_file(std::ofstream& fs, T& val) {
    fs << val;
}

template <typename T>
void to_file(std::string filename, const T& val) {
    std::ofstream fs(filename);
    to_file(fs, val);
}

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

