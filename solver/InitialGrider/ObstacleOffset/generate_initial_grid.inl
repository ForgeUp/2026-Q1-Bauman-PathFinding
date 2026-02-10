#pragma once

#include "ObstacleOffset.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads.hpp"
#include "gridgen/lazy_offset_grid_smooth.hpp"

#include "draw.hpp"


// Генерация маршрутной сети без проверки коллизии с препятствиями.
void InitialGrider::ObstacleOffset::generate_initial_grid() {
    // Генерация окаймляющих маршрутных компонент вокруг препятствий.
    Graph offset_grid = gridgen::lazy_offset_grid_smooth(task.area, corner_min, corner_max);

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
    
    draw(task, sln, "initial_grid");
}
