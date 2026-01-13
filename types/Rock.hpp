#pragma once

#include <vector>

#include "types/Point.hpp"


struct Rock {
    std::vector<Point> points;

    friend std::ostream& operator<<(std::ostream& os, const Rock& r) {
        for (auto& [x, y] : r.points) {
            os << x << ' ' << y << '\n';
        }
        os << '\n';
        return os;
    }
};
