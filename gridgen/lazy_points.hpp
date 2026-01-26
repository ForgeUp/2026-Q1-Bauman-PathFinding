#pragma once

#include <cmath>
#include <random>

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "random/from_range.hpp"


namespace gridgen {

// Генерация множества точек без проверки коллизии.
Graph lazy_points(int32_t nodes_count, const Point& min, const Point& max) {
    std::random_device rd;
    std::mt19937 gen(rd());

    Graph points;

    // Количество прямоугольников вдоль осей для размещения точек.
    int32_t cells_per_side = std::sqrt(nodes_count);

    // Остаток точек, которые будут распределены случайным образом, если вдоль оси не помещается целое число квадратов.
    int32_t remain_points_count = std::max(nodes_count - cells_per_side * cells_per_side, 0);

    // Размеры ячейки.
    double dx = (max.x - min.x) / cells_per_side;
    double dy = (max.y - min.y) / cells_per_side;

    // Границы ячейки.
    double y_l{-dy}, y_r{0};
    double x_l{-dx}, x_r{0};

    // Распределение точек по ячейкам.
    for (int64_t i = 0; i < cells_per_side; ++i) { // Цикл вдоль оси Y.
        y_l += dy; y_r += dy;
        x_l = -dx; x_r = 0;
        for (int64_t j = 0; j < cells_per_side; ++j) { // Цикл вдоль оси X.
            x_l += dx; x_r += dx;

            Point p (
                random::from_range(x_l, x_r, gen),
                random::from_range(y_l, y_r, gen)
            );

            points.add(p);
        }
    }

    // Распределение оставшихся точек.
    for (int32_t i = 0; i < remain_points_count; ++i) {
        Point p (
            random::from_range(min.x, max.x, gen),
            random::from_range(min.y, max.y, gen)
        );

        points.add(p);
    }

    return points;
}

}
