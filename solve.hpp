#pragma once

#include <iostream>
#include <format>
#include <algorithm>
#include <numeric>

#include "types/Task.hpp"
#include "types/Solution.hpp"
#include "types/SolverSettings.hpp"

#include "geometry/is_inside.hpp"

#include "random/from_range.hpp"
#include "random/from_norm.hpp"

#include "gridgen/lazy_points.hpp"
#include "gridgen/lazy_roads.hpp"
#include "pathfind/lazy.hpp"

#include "draw.hpp"


Solution solve(const Task& task, const SolverSettings& stgs) {

// Используемые переменные.

    Solution sln;

    Graph& path     = sln.path;
    Graph& grid     = sln.grid;     // Граф, в котором выполняется поиск пути.
    Graph& enhance  = sln.enhance;  // Подграф добавленных вершин и ребёр.
    Graph& examined = sln.examined; // Подграф рассмотренных в процессе поиска пути дорог и вершин.
    Graph& invalid  = sln.invalid;  // Подграф, отброшенный из-за коллизии.

    Graph invalid_all;              // Подграф невалидных рёбер за все итерации основного цикла.
    Graph invalid_all_rand;         // Подграф невалидных рёбер за все итерации основного цикла, построенных на точках, сгенерированных случайно в пределах всей арены.

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

        return true;
    };

    auto enchance_graph = [&]() -> bool {
        if (!res.is_unreachable) return false;
        
        static thread_local std::mt19937 rng{std::random_device{}()};
        
        // Генерация случайных точек в пределах всей арены.
        Graph enhance_rand_points = gridgen::lazy_points(stgs.enhance_rand_nodes_count, corner_min, corner_max);
        grid.join(enhance_rand_points);
        enhance.join(enhance_rand_points);

        // Генерация случайных точек вокруг отброшенных рёбер.

        std::vector<int32_t> idxs(invalid_all_rand.edges_count);
        std::iota(idxs.begin(), idxs.end(), 0);
        if (invalid_all_rand.edges_count >= stgs.enhance_seed_nodes_count) {
            std::shuffle(idxs.begin(), idxs.end(), rng);
            idxs.resize(stgs.enhance_seed_nodes_count);
            std::sort(idxs.begin(), idxs.end());
        }
        for (int32_t i{0}, j{0}; const auto& s : invalid_all_rand.edges()) {
            if (j >= idxs.size()) break;
            if (i++ != idxs[j]) continue;
            j++;

            Point mid (
                (s.p1.x + s.p2.x) / 2,
                (s.p1.y + s.p2.y) / 2
            );

            Point t (
                s.p2.x - s.p1.x,
                s.p2.y - s.p1.y
            );

            double len = std::sqrt(t.x * t.x + t.y * t.y);

            t.x /= len;
            t.y /= len;

            // Перпендикуляр.
            Point n(-t.y, t.x);

            double sigma_parallel = 0.5 * len;
            double sigma_perp     = 0.15 * sigma_parallel;
            
            double xi_par  = random::from_norm(0.0, sigma_parallel, rng);
            double xi_perp = random::from_norm(0.0, sigma_perp, rng);

            // Перенос в глобальные координаты.
                Point q (
                mid.x + xi_par * t.x + xi_perp * n.x,
                mid.y + xi_par * t.y + xi_perp * n.y
                );
                q.is_rand = false;

                grid.add(q);
            enhance.add(q);
        }

        // Соединение рёбрами новых точек с уже имеющимися.
        grid = gridgen::lazy_roads(grid, stgs.connection_radius);
        
        // Удаление рёбер, коллизия с которыми уже была установлена.
        for (const auto& s : invalid_all.edges()) {
            grid.remove(s);
        }

        // Запись дополнительных рёбер.
        // for (const auto& p : enhance.verts) {
        //     for (const auto& q : grid.adj[p]) {
        //         enhance.add(p, q);
        //     }
        // }

        draw(task, sln, "point_enhancement");

        attempts++;
        return true;
    };

    auto check_points_collision = [&]() -> bool {
        bool has_collided_points = false;
        std::set<Point> collided_points;

        // Проверка, что нет коллизий между вершинами пути и препятствиями.
        for (auto& p : path.verts) {
            if (p.is_checked_collsn) continue; // Пропускаем вершины, для которых уже была проверена коллизия.
            
            p.is_checked_collsn = true;
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
                Segment s(p,q);
                invalid.add(s);
                invalid_all.add(s);
                if (s.is_vert_rand()) invalid_all_rand.add(s);
            }
            grid.remove(p);
        }

        draw(task, sln, "point_collision");
        
        return true;
    };

    auto check_edges_collision = [&]() -> bool {
        bool has_collided_edges = false;
        std::set<Segment> collided_edges;

        // Проверка, что нет коллизий между рёбрами пути и препятствиями.
        for (const auto& s : path.edges()) {
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
            if (e.is_vert_rand()) invalid_all_rand.add(e);
            grid.remove(e);
        }

        draw(task, sln, "edge_collision");
    
        return true;
    };

// Реализация алгоритма Lazy PRM.

    draw(task, sln, "initial");

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
        enhance  = Graph();
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
    draw(task, sln, "final");

    return sln;
}
