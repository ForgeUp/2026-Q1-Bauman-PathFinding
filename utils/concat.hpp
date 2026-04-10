#pragma once

#include <string>


template <typename ...Args>
std::string concat(Args&&... args) {
    std::ostringstream oss;
    (oss << ... << args);
    return oss.str();
};
