#pragma once

#include <random>

#include "Gauss.hpp"

#include "solver/Solver.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "geometry/mid.hpp"

#include "gridgen/lazy_roads_Knearest.hpp"


// Генерация маршрутной сети вблизи препятствий по методу нормального распределения. 
template <typename Derived>
void InitialGrider::Gauss<Derived>::generate_initial_grid() {
    auto& S = self();

    S.metric.time_in(__func__);

    std::random_device rd;
    std::mt19937 gen(rd());

    int32_t generated_count = 0;
    while (generated_count < S.stgs.initial_nodes_count) {
        Point p (
            std::uniform_real_distribution(S.corner_min.x, S.corner_max.x)(gen),
            std::uniform_real_distribution(S.corner_min.y, S.corner_max.y)(gen)
        );

        Point q ( // Генерируем 2-ую точку в пределах заданного нормального распределения.
            std::normal_distribution(p.x, S.stgs.gauss_standard_deviation)(gen),
            std::normal_distribution(p.y, S.stgs.gauss_standard_deviation)(gen)
        );

        // Проверяем, что одна из точек лежит в свободной области, а другая - в области препятствий.
        bool p_collision = S.collision(p);
        bool q_collision = S.collision(q);

        if (!(p_collision ^ q_collision)) continue; // Пропускаем неподходящие точки.

        S.grid.add(p_collision ? q : p); // Выбираем точку из свободной области.
        generated_count++;
    }
    
    S.grid.add(S.task.start);
    S.grid.add(S.task.end);
    S.grid = gridgen::lazy_roads_Knearest({}, S.grid, S.stgs.nearest_count);

    S.visual.picture({S.task, S.sln, "initial_grid"});

    S.metric.time_out(__func__);
}
