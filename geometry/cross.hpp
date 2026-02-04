#pragma once

#include "types/Point.hpp"


namespace geometry {

// Векторное произведение смещённых векторов p0p1 и q0q1.
double cross(const Point& p0, const Point& p1, const Point& q0, const Point& q1) {
    return (p1.x - p0.x) * (q1.y - q0.y) - (p1.y - p0.y) * (q1.x - q0.x);
}

// Векторное произведение смещённых векторов ab и ac.
double cross(const Point& a, const Point& b, const Point& c) {
    return cross(a, b, a, c);
}

}
