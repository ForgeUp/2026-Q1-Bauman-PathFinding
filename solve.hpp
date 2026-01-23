#pragma once

#include <iostream>
#include <format>

#include "types/Task.hpp"
#include "types/Solution.hpp"
#include "types/SolverSettings.hpp"

#include "geometry/is_inside.hpp"

#include "generate_points.hpp"
#include "generate_lazy_roads.hpp"
#include "find_path.hpp"


Solution solve(const Task& task, const SolverSettings& stgs) {
    Solution sln;

    // Проверка, что начальная и конечная точки НЕ находятся внутри многоугольника.
    for (auto& r : task.area.rocks) {
        if (geometry::is_inside(task.start, r)) {
            std::cout << "Fail: Start point is inside of polygon" << '\n';
            return {.is_fail = true};
        }
        if (geometry::is_inside(task.end, r)) {
            std::cout << "Fail: End point is inside of polygon" << '\n';
            return {.is_fail = true};
        }
    }

    // Генерация множества точек, НЕ находящихся внутри многоугольников.
    auto [points, squares] = generate_points(task, stgs);
    
    // Генерация дорог БЕЗ проверки пересечения с препятствиями.
    sln.grid = generate_lazy_roads(task, squares, stgs);

    // Поиск пути в графе дорог с проверкой валидности ребёр.
    auto [path, examined_roads, invalid_roads, is_fail] = find_path(task, sln.grid);

    if (is_fail) {
        std::cout << "Fail: Can't find the path" << '\n';
        return {.is_fail = true};
    }

    sln.path = path;
    sln.invalid_roads = invalid_roads;
    sln.examined_roads = examined_roads;

    return sln;
}
