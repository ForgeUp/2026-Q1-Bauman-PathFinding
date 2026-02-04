#pragma once

#include "types/Point.hpp"
#include "types/Segment.hpp"
#include "types/Graph.hpp"
#include "types/Area.hpp"

#include "geometry/shift.hpp"


namespace gridgen {

// Генерация множества точек вокруг препятствий без проверки коллизии.
Graph lazy_offset_grid_smooth(const Area& area, const Point& min, const Point& max) {
    Graph points;

    double delta = 1; 

    for (const auto& r : area.rocks) { // Рассмотрение всех препятствий.
        auto& ps = r.points;
        Point first_vert, prev_vert;

        for (int32_t i = 0; i < ps.size(); ++i) { // Рассмотрение всех рёбер.
            Segment s = ( // Исходное ребро.
                i == 0 ? 
                Segment {ps[ps.size()-1], ps[i]} :
                Segment {ps[i-1],         ps[i]}
            );
            Segment curr = geometry::shift(s, delta); // Смещённое ребро.

            points.add(curr.p1, curr.p2);

            if (i != 0) points.add(prev_vert, curr.p1);
            prev_vert = curr.p2;
            if (i == 0) first_vert = curr.p1;
        }
        points.add(prev_vert, first_vert);
    }

    return points;
}

}
