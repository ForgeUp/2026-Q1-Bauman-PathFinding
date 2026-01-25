#pragma once

#include <ostream>
#include <iostream>

#include "math/eq.hpp"
#include "math/trunc.hpp"


struct Point {
    double x{0};
    double y{0};

    Point() = default;
    Point(double x_, double y_) : x{math::trunc(x_)}, y{math::trunc(y_)} {}

    friend bool operator==(const Point& a, const Point& b) {
        return math::eq(a.x, b.x) && math::eq(a.y, b.y);
    }

    friend bool operator<(const Point& a, const Point& b) {
        if (!math::eq(a.x, b.x)) return a.x < b.x;
        if (!math::eq(a.y, b.y)) return a.y < b.y;
        return false;
    }

public:
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << p.x << ' ' << p.y << '\n';
        return os;
    }

};

namespace std {

template<>
struct hash<Point> {
    std::size_t operator()(const Point& p) const noexcept {
        std::size_t hx = std::hash<double>{}(p.x);
        std::size_t hy = std::hash<double>{}(p.y);
        return hx ^ (hy << 1);
    }
};

}
