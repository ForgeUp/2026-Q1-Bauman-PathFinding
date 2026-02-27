#pragma once

#include "ObstacleOffset.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads.hpp"
#include "gridgen/lazy_offset_grid_smooth.hpp"


// Генерация маршрутной сети без проверки коллизии с препятствиями.
template <typename Derived>
void InitialGrider::ObstacleOffset<Derived>::generate_initial_grid() {
    auto& S = self();

    S.metric.time_in(__func__);

    if (!is_init) {
        enhance_nodes_count = S.stgs.enhance_rand_nodes_count;
        connection_radius   = S.stgs.connection_radius;
        is_init = true;
    }

    // Генерация окаймляющих маршрутных компонент вокруг препятствий.
    Graph offset_grid = gridgen::lazy_offset_grid_smooth(S.task.area, S.corner_min, S.corner_max);

    // Генерация случайной маршрутной сети.
    Graph rand_points = gridgen::lazy_points(S.stgs.initial_nodes_count, S.corner_min, S.corner_max);
    rand_points.add(S.task.start);
    rand_points.add(S.task.end);
    Graph rand_grid   = gridgen::lazy_roads(rand_points, S.stgs.connection_radius);
    
    // Объединений случайной и окаймляющей сетей.
    S.grid = offset_grid;
    for (const auto& p : offset_grid.verts) {
        for (const auto& q: rand_points.verts) {
            if (geometry::dist(p, q) < S.stgs.connection_radius) S.grid.add(p, q);
        }
    }

    enhance_nodes_count = offset_grid.verts.size();
    int32_t cells_per_side = std::sqrt(enhance_nodes_count);
    double dx = (S.corner_max.x - S.corner_min.x) / cells_per_side;
    double dy = (S.corner_max.y - S.corner_min.y) / cells_per_side;
    connection_radius = 2 * std::max(dx,dy);
    
    S.visual.picture({S.task, S.sln, "initial_grid"});

    S.metric.time_out(__func__);
}
