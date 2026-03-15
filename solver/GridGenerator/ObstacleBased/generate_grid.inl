#pragma once

#include <random>
#include <vector>
#include <algorithm>

#include "ObstacleBased.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "math/eq.hpp"

#include "gridgen/lazy_roads_Knearest.hpp"


template <typename Derived>
Graph GridGenerator::ObstacleBased<Derived>::generate_grid(Options& opts) {
    auto& S = self();

    auto is_inside = [&S](const Point& p){
        using math::le;
        return le(S.corner_min.x, p.x) && le(p.x, S.corner_max.x) && le(S.corner_min.y, p.y) && le(p.y, S.corner_max.y);
    };

    // [TODO] Вынести в структуру настроек.
    double step = 1;   // Шаг по вектору нормали для поиска внешней точки.
    double gap = 1;    // Зазор между внутренней и внешней точками.
    double offset = 1; // Отступ от границы препятствия для установки точки на луче.
    int32_t rays_count = 8;

    std::random_device rd;
    std::mt19937 gen(rd());

    Graph result;

    int32_t generated_count = 0;
    while (generated_count < opts.nodes_count) {
        Point p (
            std::uniform_real_distribution(S.corner_min.x, S.corner_max.x)(gen),
            std::uniform_real_distribution(S.corner_min.y, S.corner_max.y)(gen)
        ); // Генерируем точку.

        if (!S.collision(p)) continue; // Не рассматриваем точку, если она не лежит внутри препятствия.
        // Иначе точка лежит внутри препятствия.

        std::vector<double> angles(rays_count); // Случайным образом генерируем направления лучей.
        std::generate(angles.begin(), angles.end(), [&gen]{ return std::uniform_real_distribution(0.0, 180.0)(gen); });
        
        for (const auto& ang : angles) { // Цикл по всем лучам.
            double dx{std::cos(ang)}, dy{std::sin(ang)}; // Проекции вектора нормали.

            Point q(p); // Внешняя точка.

            bool is_found = false;
            while (!is_found && is_inside(q)) { // Определяем точку вне препятствия.
                q.x += dx * step;
                q.y += dy * step; // Смещаем искомую точку от внутренней точки вдоль луча.

                is_found = !S.collision(q); // Проверяем, что искомая точка лежит в свободной области.
            }
            if (!is_found) continue; // Пропускаем луч, если при поиске внешней точки вышли за границы исследуемой области (препятствие примыкает к границе области).
            // Иначе внешняя точка найдена.

            while (geometry::dist(p, q) > gap) { // Ищем границу препятствия бинарным поиском.
                Point m = geometry::mid(p, q);
                (S.collision(m) ? p : q) = m;
            }
            // Граница препятствия найдена.

            Point r (
                q.x += dx * offset,
                q.y += dy * offset
            ); // Проставляем точку вблизи границы препятствия на луче. 

            if (!is_inside(r)) continue; // Пропускаем точку, если она не лежит внутри исследуемой области.
            // Иначе найдена точка близи препятствия.

            result.add(r);
            generated_count++;
        }
    }
    
    if (opts.connect && opts.lazy) {
        result = gridgen::lazy_roads_Knearest({}, result, S.stgs.nearest_count);
    } else if (opts.connect && !opts.lazy) {
        result = gridgen::lazy_roads_Knearest({}, result, S.stgs.nearest_count); // [TODO] Заменить на вариант с проверкой коллизии.
    }

    S.visual.picture({S.task, {.enhance = result}, "obstacle_based_grid"});

    return result;
}
