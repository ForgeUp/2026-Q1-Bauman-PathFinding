#pragma once

#include <cmath>

#include "types/Point.hpp"


namespace pathfind {
namespace impl {

double heuristic(const Point& a, const Point& b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

}
}
