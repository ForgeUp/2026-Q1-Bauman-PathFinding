#pragma once

#include <set>

#include "Naive.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "geometry/is_inside.hpp"


bool CollisionChecker::Naive::check_points_collision() {
    metric.time_in(__func__);

    bool has_collided_points = false;
    std::set<Point> collided_points;

    // Проверка, что нет коллизий между вершинами пути и препятствиями.
    for (auto& p : path.verts) {
        if (p.is_checked_collsn) continue; // Пропускаем вершины, для которых уже была проверена коллизия.
        
        p.is_checked_collsn = true;
        for (auto& r : task.area.rocks) {
            if (!geometry::is_inside(p, r)) continue;
            has_collided_points = true;
            collided_points.insert(p);
            break;
        }
    }
    
    // Если коллизий не обнаружено, блок завершается.
    if (!has_collided_points) {
        metric.time_out(__func__);
        return false;
    }
    // Иначе.
    
    // Невалидные вершины (вместе с инцидентными рёбрами) удаляются из маршрутной карты.
    for (auto& p : collided_points) { // Запись удалённых вершин и рёбер.
        for (auto& q : grid.adj[p]) {
            Segment s(p,q);
            invalid.add(s);
            invalid_all.add(s);
            if (s.is_vert_rand()) invalid_all_rand.add(s);
        }
        grid.remove(p);
    }

    // visual.picture({task, sln, "point_collision"});

    metric.time_out(__func__);
    
    return true;
}
