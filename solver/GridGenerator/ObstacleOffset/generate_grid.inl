#pragma once

#include "ObstacleOffset.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "gridgen/lazy_offset_grid_sharp.hpp"


// Генерация маршрутной сети без проверки коллизии с препятствиями.
template <typename Derived>
Graph GridGenerator::ObstacleOffset<Derived>::generate_grid(Options& opts) {
    auto& S = self();

    S.metric.time_in(__func__);

    // Генерация окаймляющих маршрутных компонент вокруг препятствий.
    Graph result = gridgen::lazy_offset_grid_sharp(S.task.area, S.corner_min, S.corner_max);

    if (!opts.lazy) {} // [TODO] Удаление коллидирующих вершин и рёбер.
    
    S.visual.picture({S.task, {.enhance = result}, "obstacle_grid"});

    S.metric.time_out(__func__);

    return result;
}
