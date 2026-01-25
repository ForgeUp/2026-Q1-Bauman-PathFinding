#pragma once

#include "types/Segment.hpp"
#include "types/Rock.hpp"
#include "types/Area.hpp"

#include "is_inside.hpp"
#include "on_segment.hpp"
#include "cross.hpp"


namespace geometry {

// Проверка пересечения двух отрезков.
bool intersect(const Segment& s1, const Segment& s2) {
    const Point& a = s1.p1;
    const Point& b = s1.p2;
    const Point& c = s2.p1;
    const Point& d = s2.p2;

    double c1 = geometry::cross(a, b, c);
    double c2 = geometry::cross(a, b, d);
    double c3 = geometry::cross(c, d, a);
    double c4 = geometry::cross(c, d, b);

    if (c1 * c2 < 0 && c3 * c4 < 0) return true;

    const double EPS = 1e-9;
    if (std::fabs(c1) < EPS && geometry::on_segment(a, b, c)) return true;
    if (std::fabs(c2) < EPS && geometry::on_segment(a, b, d)) return true;
    if (std::fabs(c3) < EPS && geometry::on_segment(c, d, a)) return true;
    if (std::fabs(c4) < EPS && geometry::on_segment(c, d, b)) return true;

    return false;
}

// Проверка пересечения отрезка и препятствия.
bool intersect(const Segment& seg, const Rock& rock) {
    int64_t n = rock.points.size();

    // Проверка пересечения с рёбрами многоугольника.
    for (int64_t i = 0; i < n; ++i) {
        Segment edge{
            rock.points[i],
            rock.points[(i + 1) % n]
        };
        if (geometry::intersect(seg, edge)) return true;
    }

    // Проверка, что отрезок целиком внутри.
    if (geometry::is_inside(seg.p1, rock)) return true;

    return false;
}

// Проверка пересечения отрезка и всех препятствий внутри.
bool intersect(const Segment& s, const Area& area) {
    for (const auto& rock : area.rocks) {
        if (geometry::intersect(s, rock)) {
            return true;
        }
    }
    return false;
}

}
