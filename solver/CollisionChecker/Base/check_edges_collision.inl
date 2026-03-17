#pragma once

#include <set>

#include "Base.hpp"

#include "types/Point.hpp"
#include "types/Segment.hpp"
#include "types/Graph.hpp"


template <typename Derived>
bool CollisionChecker::Base<Derived>::check_edges_collision() {
    auto& S = self();

    bool has_collided_edges = false;
    std::set<Segment> collided_edges;

    for (const auto& s : S.path.edges()) { // Выявление коллидирующих рёбер.
        if (!S.collision(s)) continue;
        has_collided_edges = true;
        collided_edges.insert(s);
    }

    if (!has_collided_edges) return false; // Если коллизей не обнаружено, блок завершается.
    // Иначе обнаружены коллизии.

    for (auto& e : collided_edges) { // Невалидные рёбра удаляются из маршрутной карты.
        S.invalid.add(e);
        S.invalid_all.add(e);
        if (e.is_vert_rand()) S.invalid_all_rand.add(e);
        S.grid.remove(e);
    }

    // Путь ищется заново.
    return true;
}
