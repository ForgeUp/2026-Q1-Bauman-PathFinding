#pragma once

#include <ostream>

#include "math/eq.hpp"


struct Point {
    double x{0};
    double y{0};

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << p.x << ' ' << p.y << '\n';
        return os;
    }

    friend bool operator==(const Point& a, const Point& b) {
        return math::eq(a.x, b.x) && math::eq(a.y, b.y);
    }

    friend bool operator<(const Point& a, const Point& b) {
        if (!math::eq(a.x, b.x)) return a.x < b.x;
        return a.y < b.y;
    }
};
