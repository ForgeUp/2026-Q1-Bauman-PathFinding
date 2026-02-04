#pragma once

#include "types/Point.hpp"
#include "types/Segment.hpp"
#include "types/Graph.hpp"
#include "types/Area.hpp"

#include "geometry/shift.hpp"
#include "geometry/line_intersection.hpp"


namespace gridgen {

// Генерация множества точек вокруг препятствий без проверки коллизии.
Graph lazy_offset_grid_sharp(const Area& area, const Point& min, const Point& max) {
    Graph points;

    double delta = 1; 

    for (const auto& r : area.rocks) { // Рассмотрение всех препятствий.
        auto& ps = r.points;

        // Изначально смещение строится для первого и последнего сегмента многоугольника.
        Segment first = geometry::shift({ps[0],     ps[1]}, delta);
        Segment last  = geometry::shift({ps.back(), ps[0]}, delta);
        auto [first_last_p0, is_intersect] = geometry::line::intersection(last, first);
        if (!is_intersect) {
            first_last_p0 = first.p1;
            points.add(last.p2, first.p1);
        }

        Segment prev = first;
        Point prev_p0 = is_intersect ? first_last_p0 : first.p1;
        
        for (int32_t i = 1; i < ps.size(); ++i) { // Рассмотрение всех рёбер. Начинаем со 2-го ребра.
            Segment s {ps[i], ps[(i+1) % ps.size()]}; // Исходное ребро.
            Segment curr = geometry::shift(s, delta); // Смещённое ребро.

            auto [curr_p0, is_intersect] = geometry::line::intersection(prev, curr);
            if (is_intersect) {
                points.add(prev_p0, curr_p0);
                prev_p0 = curr_p0;
            } else {
                points.add(prev_p0, prev.p2);
                points.add(prev.p2, curr.p1); // Вставляем ребро-перемычку.
                prev_p0 = curr.p1;                
            }
            prev = curr;
        }
        points.add(first_last_p0, prev_p0); // Добавляем последнее ребро.
    }

    return points;
}

}
