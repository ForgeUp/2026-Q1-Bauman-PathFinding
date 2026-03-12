#pragma once

#include "types/Segment.hpp"
#include "types/Rock.hpp"
#include "types/Area.hpp"

#include "is_inside.hpp"
#include "on_segment.hpp"
#include "cross.hpp"
#include "overlap.hpp"


namespace geometry {

// Проверка пересечения двух отрезков.
bool intersect(const Segment& s1, const Segment& s2) {
    const Point& a = s1.p1;
    const Point& b = s1.p2;
    const Point& c = s2.p1;
    const Point& d = s2.p2;
    
    const double EPS = 1e-9;

    auto orient = [EPS](const Point& a, const Point& b, const Point& c)
    {
        double v = geometry::cross(a,b,c);
        if (v >  EPS) return  1;
        if (v < -EPS) return -1;
        return 0;
    };

    int o1 = orient(a,b,c);
    int o2 = orient(a,b,d);
    int o3 = orient(c,d,a);
    int o4 = orient(c,d,b);

    // Обычное пересечение.
    if (o1 != o2 && o3 != o4) return true;

    // Случаи коллинеарности.
    if (o1 == 0 && overlap(a.x,b.x,c.x,c.x) && overlap(a.y,b.y,c.y,c.y)) return true;
    if (o2 == 0 && overlap(a.x,b.x,d.x,d.x) && overlap(a.y,b.y,d.y,d.y)) return true;
    if (o3 == 0 && overlap(c.x,d.x,a.x,a.x) && overlap(c.y,d.y,a.y,a.y)) return true;
    if (o4 == 0 && overlap(c.x,d.x,b.x,b.x) && overlap(c.y,d.y,b.y,b.y)) return true;

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

// Проверка пересечения двух препятствий.
bool intersect(const Rock& r1, const Rock& r2) {
    int64_t n = r1.points.size();

    // Цикл по всем рёбрам.
    for (int64_t i = 0; i < n; ++i) {
        Segment edge{
            r1.points[i],
            r1.points[(i + 1) % n]
        };
        if (geometry::intersect(edge, r2)) return true;
    }

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
