#pragma once

#include <set>

#include "Base.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"


template <typename Derived>
bool CollisionChecker::Base<Derived>::check_points_collision() {
    auto& S = self();

    bool has_collided_points = false;
    std::set<Point> collided_points;

    for (auto& p : S.path.verts) { // Выявление коллидирующих вершин.
        if (!S.collision(p)) continue;

        has_collided_points = true;
        collided_points.insert(p);
    }
    
    if (!has_collided_points) return false; // Если коллизей не обнаружено, блок завершается.
    // Иначе обнаружены коллизии.

    for (auto& p : collided_points) { // Коллидирующие вершины (вместе с инцидентными рёбрами) удаляются из маршрутной карты.
        for (auto& q : S.grid.adj[p]) {
            Segment s(p,q);
            S.invalid.add(s);
            S.invalid_all.add(s);
            if (s.is_vert_rand()) S.invalid_all_rand.add(s);
        }
        S.grid.remove(p);
    }

    return true;
}
