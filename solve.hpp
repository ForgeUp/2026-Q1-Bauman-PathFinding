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

// Используемые переменные.

    Solution sln;

    std::vector<Point>& path = sln.path;
    Graph& grid     = sln.grid;     // Граф, в котором выполняется поиск пути.
    Graph& examined = sln.examined; // Подграф рассмотренных в процессе поиска пути дорог и вершин.
    Graph& invalid  = sln.invalid;  // Подграф, отброшенный из-за коллизии.

    Graph invalid_all;              // Подграф невалидных рёбер за все итерации основного цикла.

    int32_t attempts = 0;
    
    pathfind::Result res;

    // Левый нижний и правый верхний углы арены.
    Point corner_min(task.area.x_min, task.area.y_min);
    Point corner_max(task.area.x_max, task.area.y_max);

    // Флаги.
    bool repeat = true;
    bool terminate = false;
    bool is_found = false;

// Вспомогательные функции алгоритма Lazy PRM.

    auto check_enchancement_limit = [&]() -> bool {
        if (!res.is_unreachable) return false;
        if (attempts < stgs.enhance_attempts_limit) return false;

        draw(task, sln);

        return true;
    };

    auto enchance_graph = [&]() -> bool {
        if (!res.is_unreachable) return false;
        
        Graph enhance_points = gridgen::lazy_points(stgs.enhance_nodes_count, corner_min, corner_max);
        grid.join(enhance_points);
        grid = gridgen::lazy_roads(grid, stgs.connection_radius);
        
        // Удаление рёбер, коллизия с которыми уже была установлена.
        for (const auto& p : invalid_all.verts) {
            for (const auto& q : invalid_all.adj[p]) {
                grid.remove(p, q);
            }
        }

        attempts++;
        return true;
    };

    auto check_points_collision = [&]() -> bool {
        bool has_collided_points = false;
        std::set<Point> collided_points;

        // Проверка, что нет коллизий между вершинами пути и препятствиями.
        for (auto& p : path) {
            for (auto& r : task.area.rocks) {
                if (!geometry::is_inside(p, r)) continue;
                has_collided_points = true;
                collided_points.insert(p);
                break;
            }
        }
        
        // Если коллизий не обнаружено, блок завершается.
        if (!has_collided_points) return false;
        // Иначе.
        
        // Невалидные вершины (вместе с инцидентными рёбрами) удаляются из маршрутной карты.
        for (auto& p : collided_points) { // Запись удалённых вершин и рёбер.
            for (auto& q : grid.adj[p]) {
                invalid.add(p, q);
                invalid_all.add(p, q);
            }
            grid.remove(p);
        }

        draw(task, sln);
        
        return true;
    };

    auto check_edges_collision = [&]() -> bool {
        bool has_collided_edges = false;
        std::set<Segment> collided_edges;

        // Проверка, что нет коллизий между рёбрами пути и препятствиями.
        for (int32_t i = 1; i < path.size(); ++i) {
            const Segment s{path[i-1],path[i]};

            if (!geometry::intersect(s, task.area)) continue;
            has_collided_edges = true;
            collided_edges.insert(s);
        }

        // Если коллизей не обнаружено, блок завершается.
        if (!has_collided_edges) return false;
        // Иначе.

        // Невалидные рёбра удаляются из маршрутной карты и путь ищется заново.
        for (auto& e : collided_edges) {
            invalid.add(e);
            invalid_all.add(e);
            grid.remove(e);
        }

        draw(task, sln);
    
        return true;
    };

// Реализация алгоритма Lazy PRM.

    // Генерация множества точек БЕЗ проверки коллизии.
    Graph points = gridgen::lazy_points(stgs.initial_nodes_count, corner_min, corner_max);
    points.add(task.start);
    points.add(task.end);
    
    // Генерация дорог БЕЗ проверки коллизии.
    grid = gridgen::lazy_roads(points, stgs.connection_radius);

    while (repeat && !is_found) {
        // Поиск пути в графе дорог БЕЗ проверки коллизий вершин и ребёр с препятствиями.
        res = pathfind::lazy(task, grid);

        path     = res.path;
        examined = res.examined;
        invalid  = Graph();

        // Завершение алгоритма, если в очередной раз не удалось найти путь и иссякло число попыток для поиска.
        terminate = check_enchancement_limit();
        if (terminate) break;

        // Дополнение графа узлами и дорогами, если путь не удалось найти.
        repeat = enchance_graph();
        if (repeat) continue;
        
        // Выявление и удаление коллидирующих точек.
        repeat = check_points_collision();
        if (repeat) continue;

        // Выявление и удаление коллидирующих рёбер.
        repeat = check_edges_collision();
        if (repeat) continue;

        // Иначе путь валиден и алгоритм заканчивают свою работу.
        is_found = true;
    }

    sln.is_fail = !is_found;
    draw(task, sln);

    return sln;
}
