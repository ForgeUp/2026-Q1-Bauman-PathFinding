#pragma once

#include <random>

#include "Gauss.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"


// Генерация маршрутной сети вблизи препятствий по методу нормального распределения. 
template <typename Derived>
Graph GridGenerator::Gauss<Derived>::generate_grid(Options& opts) {
    auto& S = self();

    std::random_device rd;
    std::mt19937 gen(rd());

    Graph result;

    int32_t generated_count = 0;
    while (generated_count < opts.nodes_count) {
        Point p (
            std::uniform_real_distribution(S.corner_min.x, S.corner_max.x)(gen),
            std::uniform_real_distribution(S.corner_min.y, S.corner_max.y)(gen)
        );

        Point q ( // Генерируем 2-ую точку в пределах заданного нормального распределения.
            std::normal_distribution(p.x, S.stgs.gauss_standard_deviation)(gen),
            std::normal_distribution(p.y, S.stgs.gauss_standard_deviation)(gen)
        );

        if (q.x < S.corner_min.x || q.x > S.corner_max.x || 
            q.y < S.corner_min.y || q.y > S.corner_max.y) continue; // Пропускаем точку вне рассматриваемой области.

        // Проверяем, что одна из точек лежит в свободной области, а другая - в области препятствий.
        bool p_collision = S.collision(p);
        bool q_collision = S.collision(q);

        if (!(p_collision ^ q_collision)) continue; // Пропускаем неподходящие точки.

        result.add(p_collision ? q : p); // Выбираем точку из свободной области.
        generated_count++;
    }
    
    if (opts.connect) {
        result = S.link_grid({}, result, S.stgs.nearest_count, opts.check_collision);
    }

    S.visual.picture({S.task, {.enhance = result}, "gauss_grid"});

    return result;
}
