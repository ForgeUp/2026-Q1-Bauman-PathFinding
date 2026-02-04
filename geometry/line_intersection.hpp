#pragma once

#include "types/Segment.hpp"
#include "types/Rock.hpp"
#include "types/Area.hpp"


namespace geometry {
namespace line {

// Нахождение точки пересечения двух линий, лежащих на отрезках.
// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line
std::pair<Point,bool> intersection(const Segment& s1, const Segment& s2) {
    const auto& x1 = s1.p1.x;
    const auto& y1 = s1.p1.y;
    const auto& x2 = s1.p2.x;
    const auto& y2 = s1.p2.y;
    const auto& x3 = s2.p1.x;
    const auto& y3 = s2.p1.y;
    const auto& x4 = s2.p2.x;
    const auto& y4 = s2.p2.y;

    double den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    if (math::eq(den, 0)) return {{},false}; // Проверка на параллельность/коллинеарность.

    double m1 = x1 * y2 - y1 * x2;
    double m2 = x3 * y4 - y3 * x4;

    Point res (
        (m1 * (x3 - x4) - (x1 - x2) * m2) / den,
        (m1 * (y3 - y4) - (y1 - y2) * m2) / den
    );

    return {res,true};
}

}}
