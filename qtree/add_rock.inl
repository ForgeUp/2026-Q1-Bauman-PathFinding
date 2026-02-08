#pragma once

#include <algorithm>
// #include <cassert>
#include <set>
#include <vector>

#include "qtree.hpp"

#include "types/Point.hpp"
#include "types/Segment.hpp"
#include "types/Rock.hpp"

#include "geometry/line_intersection.hpp"

#include "math/eq.hpp"


void Qtree::add(const Rock& r) {
    // Массив рёбер.
    std::vector<Segment> segs(r.points.size());
    std::copy(r.edges().begin(), r.edges().end(), segs.begin());

    // Растеризируем рёбра.
    for (auto& s : segs) {
        rasterize(s);
    }

    // Массив для определения текущих рёбер для нахождения пересечения с горизонтальной линией.
    std::vector<std::pair<Point,Segment*>> slider;
    for (auto& s : segs) {
        slider.emplace_back(s.p1, &s);
        slider.emplace_back(s.p2, &s);
    }

    // Сортируем по вертикали снизу-вверх.
    std::sort(slider.begin(), slider.end(), [](auto& l, auto& r) {
        return l.first.y < r.first.y;
    });

    // Индекс ближайшего НЕактивного ребра.
    int32_t idx = 0;

    // Вертикальные границы растеризации.
    double y_min = slider[0].first.y;
    double y_max = slider.back().first.y;

    // Вертикальный и горизонтальный шаг растеризации.
    double dy = min_box_size_point * 0.5;
    double dx = min_box_size_point * 0.5;

    // Массив используемых рёбер.
    std::set<Segment*> active;

    for (double y = y_min + dy * 0.5; math::le(y, y_max); y += dy) { // Перебор всех горизонтальных линий.
        Segment horz(Point(0,y), Point(1,y)); // Горизонтальная линия.

        // Обновляемые активные рёбра.
        while (idx < slider.size() && math::ge(y, slider[idx].first.y)) {
            if (active.contains(slider[idx].second)) { // Если встретилось впервые - добавляем, иначе - удаляем.
                active.erase(slider[idx].second);
            } else {
                active.insert(slider[idx].second);
            }
            idx++; // Сдвигаем индекс на следующее ребро.
        }

        // Определяем границы отрезков растеризации.
        std::vector<Point> ps;
        bool skip_line = false;
        for (const auto& s : active) {
            auto [p, is_success] = geometry::line::intersection(horz, *s); // Определяем точку пересечения горизонтальной линии и ребра.
            
            if (!is_success) { // Такого произойти не должно, но тем не менее.. не закрашиваем эту линию.
                // assert(!is_success && "Fail to rasterize line");
                skip_line = true;
                break;
            }

            ps.push_back(p);
        }
        if (skip_line) continue;
        
        // Сортируем точки по горизонтали.
        std::sort(ps.begin(), ps.end(), [](auto& l, auto& r) {
            return l.x < r.x;
        });
 
        // Формируем отрезки растеризации.
        std::vector<Segment> intervals;
        bool inside = false;
        for (Point l, r; const auto& p : ps) {
            (inside ? r : l) = p;
            if (inside) intervals.emplace_back(l, r);
            inside = !inside;
        }

        // Растеризация.
        for (const auto& [pl, pr] : intervals) {
            Box* b;
            for (double x = std::clamp(pl.x, min.x, max.x); math::le(x,pr.x); x = b->x_max + dx * 0.5) {
                b = locate({x,y});
                if (b == nullptr) break;
                b->type = b->type == Type::Mix ? Type::Mix : Type::Busy;
            }
        }
    }
}
 