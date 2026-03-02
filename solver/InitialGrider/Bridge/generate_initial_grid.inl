#pragma once

#include <random>

#include "Bridge.hpp"

#include "solver/Solver.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "random/from_range.hpp"
#include "random/from_norm.hpp"

#include "geometry/mid.hpp"

#include "gridgen/lazy_roads_Knearest.hpp"


// Генерация маршрутной сети по методу BridgeTest.
template <typename Derived>
void InitialGrider::Bridge<Derived>::generate_initial_grid() {
    auto& S = self();

    S.metric.time_in(__func__);

    std::random_device rd;
    std::mt19937 gen(rd());

    int32_t generated_count = 0;
    while (generated_count < S.stgs.initial_nodes_count) {
        Point p (
            random::from_range(S.corner_min.x, S.corner_max.x, gen),
            random::from_range(S.corner_min.y, S.corner_max.y, gen)
        );

        if (!S.collision(p)) continue; // Пропускаем точку из свободной области.
        // Иначе точка находится в области препятствий.

        Point q ( // Генерируем 2-ую точку в пределах заданного нормального распределения.
            random::from_norm(p.x, S.stgs.bridge_standard_deviation, gen),
            random::from_norm(p.y, S.stgs.bridge_standard_deviation, gen)
        );

        if (!S.collision(q)) continue; // Пропускаем 2-ую точку из свободной области.
        // Иначе 2-ая точка находится в области препятствий.

        Point m = geometry::mid(p, q); // Определяем центральную точку.

        if (!S.collision(m)) continue; // Пропускаем центральную точку, если она находится в свободной области.
        // Иначе найдена точка из узкого прохода.

        S.grid.add(m);
        generated_count++;
    }
    
    S.grid.add(S.task.start);
    S.grid.add(S.task.end);
    S.grid = gridgen::lazy_roads_Knearest({}, S.grid, S.stgs.nearest_count);

    S.visual.picture({S.task, S.sln, "initial_grid"});

    S.metric.time_out(__func__);
}
