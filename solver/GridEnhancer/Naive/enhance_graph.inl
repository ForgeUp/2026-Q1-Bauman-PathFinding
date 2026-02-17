#pragma once

#include <cmath>

#include "Naive.hpp"

#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads.hpp"


// Усиление графа путём уплотнения сетки случайными точками с удвоения количества на каждом шаге.
bool GridEnhancer::Naive::enhance_graph() {
    if (!is_path_not_found) return false;

    // Генерация случайных точек в пределах всей арены.
    Graph enhance_rand_points = gridgen::lazy_points(enhance_nodes_count, corner_min, corner_max);

    grid.join(enhance_rand_points);
    enhance.join(enhance_rand_points);

    // Соединение рёбрами новых точек с уже имеющимися.
    grid = gridgen::lazy_roads(grid, connection_radius);
    
    // Удаление рёбер, коллизия с которыми уже была установлена.
    for (const auto& s : invalid_all.edges()) {
        grid.remove(s);
    }

    // Увеличение количества точек для следующей итерации.
    enhance_nodes_count *= 2;
    
    // Количество прямоугольников вдоль осей для размещения точек.
    int32_t cells_per_side = std::sqrt(enhance_nodes_count);
    // Размеры ячейки.
    double dx = (corner_max.x - corner_min.x) / cells_per_side;
    double dy = (corner_max.y - corner_min.y) / cells_per_side;

    // Уменьшение радиуса связывания, так как узлы ближе.
    connection_radius = 2 * std::max(dx,dy);

    visual.picture({task, sln, "point_enhancement"});

    attempts++;
    return true;
}
