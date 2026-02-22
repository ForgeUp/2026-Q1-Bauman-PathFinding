#pragma once

#include "ClusterOffset.hpp"

#include "types/Point.hpp"
#include "types/Area.hpp"
#include "types/Graph.hpp"

#include "cluster/compose.hpp"
#include "cluster/graham.hpp"
#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads.hpp"
#include "gridgen/lazy_offset_grid_sharp.hpp"


// Генерация маршрутной сети без проверки коллизии с препятствиями.
void InitialGrider::ClusterOffset::generate_initial_grid() {
    metric.time_in(__func__);

    // Выявление кластеров препятствий.
    auto clusters = cluster::compose(task.area.rocks);

    // Создание сцены с кластерами.
    Area cluster_area;
    cluster_area.rocks.resize(clusters.size());
    for (int32_t i = 0; i < clusters.size(); ++i) {
        cluster_area.rocks[i] = cluster::graham(clusters[i].rocks);
    }

    // Генерация окаймляющих маршрутных компонент вокруг кластеров препятствий.
    Graph offset_grid = gridgen::lazy_offset_grid_sharp(cluster_area, corner_min, corner_max);

    visual.picture({task, {.grid = offset_grid}, "offset_grid"});

    // Генерация случайной маршрутной сети.
    Graph rand_points = gridgen::lazy_points(stgs.initial_nodes_count, corner_min, corner_max);
    rand_points.add(task.start);
    rand_points.add(task.end);
    Graph rand_grid   = gridgen::lazy_roads(rand_points, stgs.connection_radius);
    
    // Объединений случайной и окаймляющей сетей.
    grid = offset_grid;
    for (const auto& p : offset_grid.verts) {
        for (const auto& q: rand_points.verts) {
            if (geometry::dist(p, q) < stgs.connection_radius) grid.add(p, q);
        }
    }

    enhance_nodes_count = offset_grid.verts.size();
    int32_t cells_per_side = std::sqrt(enhance_nodes_count);
    double dx = (corner_max.x - corner_min.x) / cells_per_side;
    double dy = (corner_max.y - corner_min.y) / cells_per_side;
    connection_radius = 2 * std::max(dx,dy);
    
    visual.picture({task, sln, "initial_grid"});

    metric.time_out(__func__);
}

