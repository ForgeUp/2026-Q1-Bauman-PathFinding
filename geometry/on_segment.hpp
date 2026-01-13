#pragma once

#include "types/Point.hpp"
#include "types/Segment.hpp"

#include "math/eq.hpp"


namespace geometry {

bool on_segment(const Point& p, const Point& a, const Point& b) {
    double cross =
        (p.x - a.x) * (b.y - a.y) -
        (p.y - a.y) * (b.x - a.x);

    if (!math::eq(cross, 0)) return false;

    double dot =
        (p.x - a.x) * (b.x - a.x) +
        (p.y - a.y) * (b.y - a.y);

    if (dot < 0) return false;

    double lenSq =
        (b.x - a.x) * (b.x - a.x) +
        (b.y - a.y) * (b.y - a.y);

    return dot <= lenSq;
}

bool on_segment(const Point& p, const Segment& s) {
    return on_segment(p, s.p1, s.p2);
}

}
