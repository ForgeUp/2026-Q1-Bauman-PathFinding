#pragma once

#include <random>

#include "Bridge.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "geometry/mid.hpp"

#include "gridgen/lazy_roads_Knearest.hpp"


// Генерация маршрутной сети по методу BridgeTest.
template <typename Derived>
Graph GridGenerator::Bridge<Derived>::generate_grid(Options& opts) {
    auto& S = self();

    S.metric.time_in(__func__);

    std::random_device rd;
    std::mt19937 gen(rd());

    Graph result;

    int32_t generated_count = 0;
    while (generated_count < opts.nodes_count) {
        Point p (
            std::uniform_real_distribution(S.corner_min.x, S.corner_max.x)(gen),
            std::uniform_real_distribution(S.corner_min.y, S.corner_max.y)(gen)
        );

        if (!S.collision(p)) continue; // Пропускаем точку из свободной области.
        // Иначе точка находится в области препятствий.

        Point q ( // Генерируем 2-ую точку в пределах заданного нормального распределения.
            std::normal_distribution(p.x, S.stgs.bridge_standard_deviation)(gen),
            std::normal_distribution(p.y, S.stgs.bridge_standard_deviation)(gen)
        );

        if (!S.collision(q)) continue; // Пропускаем 2-ую точку из свободной области.
        // Иначе 2-ая точка находится в области препятствий.

        Point m = geometry::mid(p, q); // Определяем центральную точку.

        if (S.collision(m)) continue; // Пропускаем центральную точку, если она находится в свободной области.
        // Иначе найдена точка из узкого прохода.

        result.add(m);
        generated_count++;
    }
    
    if (opts.connect && opts.lazy) {
        result = gridgen::lazy_roads_Knearest({}, result, S.stgs.nearest_count);
    } else if (opts.connect && !opts.lazy) {
        result = gridgen::lazy_roads_Knearest({}, result, S.stgs.nearest_count); // [TODO] Заменить на вариант с проверкой коллизии.
    }

    S.visual.picture({S.task, {.enhance = result}, "bridge_grid"});

    S.metric.time_out(__func__);

    return result;
}
