#pragma once

#include "types/Point.hpp"
#include "types/Rock.hpp"

#include "on_segment.hpp"


namespace geometry {

bool is_inside(const Point& p, const Rock& r) {
    const auto& poly = r.points;
    if (poly.size() < 3) return false;

    bool inside = false;

    for (size_t i = 0, j = poly.size() - 1; i < poly.size(); j = i++) {
        const Point& a = poly[j];
        const Point& b = poly[i];

        if (geometry::on_segment(p, a, b)) return true;

        bool intersect =
            ((a.y > p.y) != (b.y > p.y)) &&
            (p.x < (b.x - a.x) * (p.y - a.y) / (b.y - a.y) + a.x);

        if (intersect) inside = !inside;
    }

    return inside;
}

bool is_inside(const Point& p, const std::vector<Rock>& rs) {
    for (auto& r : rs) {
        if (is_inside(p, r)) return true;
    }
    return false;
}

bool is_inside(const std::vector<Point>& ps, const std::vector<Rock>& rs) {
    for (auto& p : ps) {
        if (is_inside(p, rs)) return true;
    }
    return false;
}

}
