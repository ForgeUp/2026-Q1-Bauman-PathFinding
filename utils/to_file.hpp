#pragma once

#include <fstream>
#include <vector>
#include <set>
#include <type_traits>


namespace io {

struct Row {
    std::vector<double> data;

    friend std::ofstream& operator<<(std::ofstream& os, const Row& row) {
        for (const auto& val : row.data) {
            os << val << ' ';
        } 
        return os;
    }
};

struct Options {
    std::vector<std::string> head;
};

template <typename T>
void append(std::ofstream& fs, const T& val) {
    fs << val << '\n';
}

template <typename T1, typename T2>
void append(std::ofstream& fs, const std::pair<T1,T2>& p) {
    fs << p.first << ' ' << p.second << '\n';
}

void append(std::ofstream& fs, const std::string& str) {
    fs << str << '\n';
}

template <std::ranges::range Range>
void append(std::ofstream& fs, const Range& r) {
    for (const auto& obj : r) {
        append(fs, obj);
    }
}

template <typename T>
void to_file(std::ofstream& fs, const T& val, const io::Options& opts = {}) {
    if (opts.head.size() > 0) {
        for (const auto& str : opts.head) {
            fs << '"' << str << '"' << ' ';
        }
        fs << '\n';
    }
    append(fs, val);
}

template <typename T, typename Path>
    requires (!std::is_same_v<std::decay_t<Path>, std::ofstream>)
void to_file(const Path& filename, const T& val, const io::Options& opts = {}) {
    std::ofstream fs(filename);
    to_file(fs, val, opts);
}

}
