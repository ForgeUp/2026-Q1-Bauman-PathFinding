#pragma once

#include <set>

#include "Naive.hpp"

#include "types/Point.hpp"
#include "types/Segment.hpp"
#include "types/Graph.hpp"

#include "geometry/intersect.hpp"


bool CollisionChecker::Naive::check_edges_collision() {
    metric.time_in(__func__);

    bool has_collided_edges = false;
    std::set<Segment> collided_edges;

    // Проверка, что нет коллизий между рёбрами пути и препятствиями.
    for (const auto& s : path.edges()) {
        if (!geometry::intersect(s, task.area)) continue;
        has_collided_edges = true;
        collided_edges.insert(s);
    }

    // Если коллизей не обнаружено, блок завершается.
    if (!has_collided_edges) {
        metric.time_out(__func__);
        return false;
    }
    // Иначе.

    // Невалидные рёбра удаляются из маршрутной карты.
    for (auto& e : collided_edges) {
        invalid.add(e);
        invalid_all.add(e);
        if (e.is_vert_rand()) invalid_all_rand.add(e);
        grid.remove(e);
    }

    // visual.picture({task, sln, "edge_collision"});

    metric.time_out(__func__);

    // Путь ищется заново.
    return true;
}
