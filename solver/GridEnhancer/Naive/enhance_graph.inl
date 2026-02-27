#pragma once

#include <cmath>

#include "Naive.hpp"

#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads.hpp"


// Усиление графа путём уплотнения сетки случайными точками с удвоения количества на каждом шаге.
template <typename Derived>
bool GridEnhancer::Naive<Derived>::enhance_graph() {
    auto& S = self();

    if (!S.is_path_not_found) return false;
    
    S.metric.time_in(__func__);

    if (!is_init) {
        enhance_nodes_count = S.stgs.enhance_rand_nodes_count;
        connection_radius   = S.stgs.connection_radius;
        is_init = true;
    }

    // Генерация случайных точек в пределах всей арены.
    Graph enhance_rand_points = gridgen::lazy_points(enhance_nodes_count, S.corner_min, S.corner_max);

    S.grid.join(enhance_rand_points);
    S.enhance.join(enhance_rand_points);

    // Соединение рёбрами новых точек с уже имеющимися.
    S.grid = gridgen::lazy_roads(S.grid, connection_radius);
    
    // Удаление рёбер, коллизия с которыми уже была установлена.
    for (const auto& s : S.invalid_all.edges()) {
        S.grid.remove(s);
    }

    // Увеличение количества точек для следующей итерации.
    enhance_nodes_count *= 2;
    
    // Количество прямоугольников вдоль осей для размещения точек.
    int32_t cells_per_side = std::sqrt(enhance_nodes_count);
    // Размеры ячейки.
    double dx = (S.corner_max.x - S.corner_min.x) / cells_per_side;
    double dy = (S.corner_max.y - S.corner_min.y) / cells_per_side;

    // Уменьшение радиуса связывания, так как узлы ближе.
    connection_radius = 2 * std::max(dx,dy);

    S.visual.picture({S.task, S.sln, "point_enhancement"});

    S.metric.time_out(__func__);

    S.attempts++;
    return true;
}
