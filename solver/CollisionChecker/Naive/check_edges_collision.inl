#pragma once

#include <set>

#include "Naive.hpp"

#include "types/Point.hpp"
#include "types/Segment.hpp"
#include "types/Graph.hpp"

#include "geometry/intersect.hpp"


template <typename Derived>
bool CollisionChecker::Naive<Derived>::check_edges_collision() {
    auto& S = self();

    S.metric.time_in(__func__);

    bool has_collided_edges = false;
    std::set<Segment> collided_edges;

    // Проверка, что нет коллизий между рёбрами пути и препятствиями.
    for (const auto& s : S.path.edges()) {
        if (!geometry::intersect(s, S.task.area)) continue;
        has_collided_edges = true;
        collided_edges.insert(s);
    }

    // Если коллизей не обнаружено, блок завершается.
    if (!has_collided_edges) {
        S.metric.time_out(__func__);
        return false;
    }
    // Иначе.

    // Невалидные рёбра удаляются из маршрутной карты.
    for (auto& e : collided_edges) {
        S.invalid.add(e);
        S.invalid_all.add(e);
        if (e.is_vert_rand()) S.invalid_all_rand.add(e);
        S.grid.remove(e);
    }

    // visual.picture({task, sln, "edge_collision"});

    S.metric.time_out(__func__);

    // Путь ищется заново.
    return true;
}
