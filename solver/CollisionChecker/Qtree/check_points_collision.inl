#pragma once

#include <set>

#include "Qtree.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "geometry/is_inside.hpp"


template <typename Derived>
bool CollisionChecker::Qtree<Derived>::check_points_collision() {
    auto& S = self();

    if (!is_init) build_qtree();

    S.metric.time_in(__func__);

    bool has_collided_points = false;
    std::set<Point> collided_points;

    // Проверка, что нет коллизий между вершинами пути и препятствиями.
    for (auto& p : S.path.verts) {
        if (p.is_checked_collsn) continue; // Пропускаем вершины, для которых уже была проверена коллизия.
        
        p.is_checked_collsn = true;
        if (!qtree.collision(p)) continue;

        has_collided_points = true;
        collided_points.insert(p);
    }
    
    // Если коллизий не обнаружено, блок завершается.
    if (!has_collided_points) {
        S.metric.time_out(__func__);
        return false;
    }
    // Иначе.
    
    // Невалидные вершины (вместе с инцидентными рёбрами) удаляются из маршрутной карты.
    for (auto& p : collided_points) { // Запись удалённых вершин и рёбер.
        for (auto& q : S.grid.adj[p]) {
            Segment s(p,q);
            S.invalid.add(s);
            S.invalid_all.add(s);
            if (s.is_vert_rand()) S.invalid_all_rand.add(s);
        }
        S.grid.remove(p);
    }

    // S.visual.picture({S.task, S.sln, "point_collision"});
    
    S.metric.time_out(__func__);

    return true;
}
