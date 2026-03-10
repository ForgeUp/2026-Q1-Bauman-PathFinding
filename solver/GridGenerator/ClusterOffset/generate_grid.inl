#pragma once

#include "ClusterOffset.hpp"

#include "types/Point.hpp"
#include "types/Area.hpp"
#include "types/Graph.hpp"

#include "cluster/compose.hpp"
#include "cluster/graham.hpp"

#include "gridgen/lazy_offset_grid_sharp.hpp"


template <typename Derived>
Graph GridGenerator::ClusterOffset<Derived>::generate_grid(Options& opts) {
    auto& S = self();

    S.metric.time_in(__func__);

    // Выявление кластеров препятствий.
    auto clusters = cluster::compose(S.task.area.rocks);

    // Создание сцены с кластерами.
    Area cluster_area;
    cluster_area.rocks.resize(clusters.size());
    for (int32_t i = 0; i < clusters.size(); ++i) {
        cluster_area.rocks[i] = cluster::graham(clusters[i].rocks);
    }

    // Генерация окаймляющих маршрутных компонент вокруг кластеров препятствий.
    Graph result = gridgen::lazy_offset_grid_sharp(cluster_area, S.corner_min, S.corner_max);

    if (!opts.lazy) {} // [TODO] Удаление коллидирующих вершин и рёбер.
    
    S.visual.picture({S.task, {.enhance = result}, "cluster_grid"});

    S.metric.time_out(__func__);

    return result;
}

