#pragma once

#include "Uniform.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads_Knearest.hpp"


// Генерация маршрутной сети без проверки коллизии с препятствиями.
template <typename Derived>
Graph GridGenerator::Uniform<Derived>::generate_grid(Options& opts) {
    auto& S = self();

    S.metric.time_in(__func__);
    
    Graph result;
    if (opts.lazy) {
        result = gridgen::lazy_points(opts.nodes_count, S.corner_min, S.corner_max);
    } else {
        result = gridgen::lazy_points(opts.nodes_count, S.corner_min, S.corner_max); // [TODO] Заменить на вариант с проверкой коллизии.
    }
    
    if (opts.connect && opts.lazy) {
        result = gridgen::lazy_roads_Knearest({}, result, S.stgs.nearest_count);
    } else if (opts.connect && !opts.lazy) {
        result = gridgen::lazy_roads_Knearest({}, result, S.stgs.nearest_count); // [TODO] Заменить на вариант с проверкой коллизии.
    }
    
    S.visual.picture({S.task, {.enhance = result}, "uniform_grid"});

    S.metric.time_out(__func__);

    return result;
}
