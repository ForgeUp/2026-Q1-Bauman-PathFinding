#pragma once

#include <set>

#include "Qtree.hpp"

#include "types/Point.hpp"
#include "types/Segment.hpp"
#include "types/Graph.hpp"

#include "geometry/intersect.hpp"


bool CollisionChecker::Qtree::check_edges_collision() {
    if (!qtree_ready) {
        for (const auto& r : task.area.rocks) {
            qtree.add(r);
        }
        qtree_ready = true;
        visual.picture({task, {.qtree = qtree}, "qtree"});
    }

    bool has_collided_edges = false;
    std::set<Segment> collided_edges;

    // Проверка, что нет коллизий между рёбрами пути и препятствиями.
    for (const auto& s : path.edges()) {
        if (!qtree.collision(s)) continue;
        has_collided_edges = true;
        collided_edges.insert(s);
    }

    // Если коллизей не обнаружено, блок завершается.
    if (!has_collided_edges) return false;
    // Иначе.

    // Невалидные рёбра удаляются из маршрутной карты.
    for (auto& e : collided_edges) {
        invalid.add(e);
        invalid_all.add(e);
        if (e.is_vert_rand()) invalid_all_rand.add(e);
        grid.remove(e);
    }

    // visual.picture({task, sln, "edge_collision"});

    // Путь ищется заново.
    return true;
}
