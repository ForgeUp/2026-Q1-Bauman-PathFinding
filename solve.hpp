#pragma once

#include <iostream>
#include <format>

#include "types/Task.hpp"
#include "types/Solution.hpp"
#include "types/SolverSettings.hpp"

#include "geometry/is_inside.hpp"

#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads.hpp"
#include "pathfind/lazy.hpp"

#include "draw.hpp"


Solution solve(const Task& task, const SolverSettings& stgs) {
    Solution sln;

    std::vector<Point> path;
    Graph grid;     // Граф, в котором выполняется поиск пути.
    Graph examined; // Граф рассмотренных в процессе поиска пути дорог и вершин.
    Graph invalid;  // Подграф, отброшенный из-за коллизии.
    
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

    // Генерация множества точек БЕЗ проверки коллизии.
    auto [points, squares] = gridgen::lazy_points(task, stgs);
    
    // Генерация дорог БЕЗ проверки коллизии.
    grid = gridgen::lazy_roads(task, squares, stgs);
    
    bool is_found = false;
    while (!is_found) {
        // Поиск пути в графе дорог БЕЗ проверки коллизий вершин и ребёр с препятствиями.
        auto res = pathfind::lazy(task, grid);
        examined = res.examined;
        invalid = Graph();
        path = res.path;

        // Проверка, что путь удалось найти.
        if (res.is_unreachable) {
            // [TODO] Реализовать дополнением новыми вершинами и повтор алгоритма в случае неудачи.
            std::cout << "Fail: Can't find the path" << '\n';
            sln = {
                .path = path,
                .grid = grid,
                .examined = examined,
                .invalid = invalid,
                .is_fail = true
            };
            draw(task, sln);
            return sln;
        }

        // Проверка, что нет коллизий между вершинами пути и препятствиями.
        bool has_collided_points = false;
        std::set<Point> collided_points;
        for (auto& p : path) {
            for (auto& r : task.area.rocks) {
                if (!geometry::is_inside(p, r)) continue;
                has_collided_points = true;
                collided_points.insert(p);
                break;
            }
        }

        // Если присутвуют невалидные вершины, то они (вместе с инцидентными рёбрами) удаляются из маршрутной карты, и путь ищется заново.
        if (has_collided_points) {
            for (auto& p : collided_points) { // Запись удалённых вершин и рёбер.
                for (auto& q : grid.adj[p]) {
                    invalid.add(p, q);
                }
                grid.remove(p);
            }
            sln = {
                .path = path,
                .grid = grid,
                .examined = examined,
                .invalid = invalid,
                .is_fail = true
            };
            draw(task, sln);
            continue;
        }

        // Проверка, что нет коллизий между рёбрами пути и препятствиями.
        bool has_collided_edges = false;
        std::set<Segment> collided_edges;
        for (int32_t i = 1; i < path.size(); ++i) {
            const Segment s{path[i-1],path[i]};

            if (!geometry::intersect(s, task.area)) continue;
            has_collided_edges = true;
            collided_edges.insert(s);
        }

        // Если присутвуют невалидные рёбра, то они удаляются из маршрутной карты, и путь ищется заново.
        if (has_collided_edges) {
            for (auto& e : collided_edges) {
                invalid.add(e);
                grid.remove(e);
            }
            sln = {
                .path = path,
                .grid = grid,
                .examined = examined,
                .invalid = invalid,
                .is_fail = true
            };
            draw(task, sln);
            continue;
        }

        // Иначе путь валиден.
        is_found = true;
    }

    sln = {
        .path = path,
        .grid = grid,
        .examined = examined,
        .invalid = invalid,
        .is_fail = true
    };
    draw(task, sln);
    return sln;
}
