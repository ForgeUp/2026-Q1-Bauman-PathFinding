#pragma once

#include <vector>
#include <map>
#include <random>

#include "types/Task.hpp"
#include "types/Solution.hpp"
#include "types/SolverSettings.hpp"
#include "types/Segment.hpp"

#include "math/in_range.hpp"

#include "random/from_range.hpp"

#include "geometry/is_inside.hpp"



namespace gridgen {

using square_idx = std::pair<int64_t,int64_t>;

// Генерация множества точек без проверки коллизии.
auto lazy_points(const Task& task, const SolverSettings& stgs) {
    std::random_device rd;
    std::mt19937 gen(rd());

    // Размер ячейки.
    double dx = (task.area.x_max - task.area.x_min) / stgs.squares_per_side;
    double dy = (task.area.y_max - task.area.y_min) / stgs.squares_per_side;

    // Границы ячейки.
    double y_l{-dy}, y_r{0};
    double x_l{-dx}, x_r{0};

    // Множество ячеек.
    // Ключ: пара из индексов ячейки вдоль оси Y и оси X соответственно.
    // Значение: точка внутри ячейки.
    // Если ячейки нет в множестве, то точку НЕ удалось разместить внутри ячейки.
    std::map<square_idx,Point> squares;

    // Обычный массив точек для удобства.
    std::vector<Point> points;

    for (int64_t i = 0; i < stgs.squares_per_side; ++i) { // Цикл вдоль оси Y.
        y_l += dy; y_r += dy;
        x_l = -dx; x_r = 0;
        for (int64_t j = 0; j < stgs.squares_per_side; ++j) { // Цикл вдоль оси X.
            x_l += dx; x_r += dx;
            
            // [TODO] Добавить обработку случая, когда стартовая и конечная точки находятся в одной ячейке.
            // Если данная ячейка включает стартовую или конечную точку, то выбираем её, иначе генерируем случайную.
            Point p = (
                math::in_range(task.start.x, x_l, x_r) && math::in_range(task.start.y, y_l, y_r) ? task.start :
                math::in_range(task.end.x  , x_l, x_r) && math::in_range(task.end.y  , y_l, y_r) ? task.end :
                Point {
                    random::from_range(x_l, x_r, gen),
                    random::from_range(y_l, y_r, gen)
                }
            );

            // Добавляем ячейку и точку.
            squares[{i,j}] = p;
            points.emplace_back(p);
        }
    }

    return std::pair(points, squares);
}

}
