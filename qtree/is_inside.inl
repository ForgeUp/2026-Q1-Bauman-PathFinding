#pragma once

#include <cmath>

#include "qtree.hpp"

#include "types/Point.hpp"

#include "math/eq.hpp"


bool Qtree::Box::is_inside(const Point& p) {
    using math::le;
    return le(x_min, p.x) && le(p.x, x_max) && le(y_min, p.y) && le(p.y, y_max);
}

bool Qtree::Box::is_inside_q1(const Point& p) {
    using math::le;
    return le(x_mid, p.x) && le(p.x, x_max) && le(y_mid, p.y) && le(p.y, y_max);
}

bool Qtree::Box::is_inside_q2(const Point& p) {
    using math::le;
    return le(x_min, p.x) && le(p.x, x_mid) && le(y_mid, p.y) && le(p.y, y_max);
}

bool Qtree::Box::is_inside_q3(const Point& p) {
    using math::le;
    return le(x_min, p.x) && le(p.x, x_mid) && le(y_min, p.y) && le(p.y, y_mid);
}

bool Qtree::Box::is_inside_q4(const Point& p) {
    using math::le;
    return le(x_mid, p.x) && le(p.x, x_max) && le(y_min, p.y) && le(p.y, y_mid);
}

double Qtree::Box::length() {
    return std::min(x_max-x_min, y_max-y_min);
}
        