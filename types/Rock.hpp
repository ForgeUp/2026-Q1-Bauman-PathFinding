#pragma once

#include <vector>

#include "types/Point.hpp"


struct Rock {
    std::vector<Point> points;

    friend std::ostream& operator<<(std::ostream& os, const Rock& r) {
        for (auto& p : r.points) {
            os << p.x << ' ' << p.y << '\n';
        }
        os << '\n';
        return os;
    }
};
