#pragma once

#include <fstream>
#include <vector>
#include <set>
#include <type_traits>


template <typename T>
void to_file(std::ofstream& fs, const T& val) {
    fs << val << '\n';
}

template <typename T1, typename T2>
void to_file(std::ofstream& fs, const std::pair<T1,T2>& p) {
    fs << p.first << ' ' << p.second << '\n';
}

template <typename T>
concept NotString = !std::is_same_v<std::decay_t<T>, std::string>;

template <std::ranges::range Range>
    requires NotString<Range>
void to_file(std::ofstream& fs, const Range& r) {
    for (const auto& obj : r) {
        to_file(fs, obj);
    }
}

template <typename T, typename Path>
void to_file(const Path& filename, const T& val) {
    std::ofstream fs(filename);
    to_file(fs, val);
}
