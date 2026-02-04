#pragma once

#include <algorithm>
#include <set>
#include <vector>

#include "types/Point.hpp"
#include "types/Rock.hpp"

#include "geometry/cross.hpp"
#include "geometry/dist.hpp"


namespace cluster {

Rock graham(const std::vector<Rock>& rocks) {
    Rock res;

    // Собираем уникальные вершины всех препятствий в один массив.
    std::vector<Point> ps;
    for (std::set<Point> cache; const auto& r : rocks) {
        for (const auto& p : r.points) {
            if (cache.contains(p)) continue;
            ps.push_back(p);
            cache.insert(p);
        }
    }

    // Проверка вырожденного случая.
    if (ps.size() < 3) {
        res.points = ps;
        return res;
    }

    // Ищем самую нижнюю точку.
    int32_t p0_idx = 0;
    for (int32_t i = 1; i < ps.size(); ++i) {
        auto& p  = ps[i];
        auto& p0 = ps[p0_idx];
        if (p.y < p0.y || p.y == p0.y && p.x < p0.x) p0_idx = i;
    }
    Point p0 = ps[p0_idx];

    // Перемещаем самую нижнюю точку в начало.
    std::swap(ps[0], ps[p0_idx]);

    // Сортируем по полярному углу.
    std::sort(ps.begin() + 1, ps.end(), [&](Point& a, Point& b){
        auto c = geometry::cross(p0, a, b);
        if (c != 0) return c > 0;

        // Если угол одинаковый - ближайшая точка.
        return geometry::dist(p0, a) < geometry::dist(p0, b);
    });
    
    // Формирование оболочки.
    std::vector<Point> hull;
    for (int32_t i = 0; i < ps.size(); ++i) {
        auto& pk = ps[i];
        while (hull.size() >= 2) {
            auto& pj = hull[hull.size()-1];
            auto& pi = hull[hull.size()-2];

            if (geometry::cross(pi, pj, pj, pk) < 0) {
                hull.pop_back();
            } else break;
        }
        hull.push_back(pk);
    }

    res.points = std::move(hull);
    return res;
}

}
