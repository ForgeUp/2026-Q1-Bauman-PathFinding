#pragma once

#include <cmath>

#include "types/Point.hpp"


namespace geometry {

double dist(const Point& a, const Point& b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

}
