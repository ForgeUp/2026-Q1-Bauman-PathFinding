#pragma once

#include <cmath>
#include <random>

#include "Uniform.hpp"

#include "types/Point.hpp"
#include "types/Graph.hpp"


// Генерация маршрутной сети без проверки коллизии с препятствиями.
template <typename Derived>
Graph GridGenerator::Uniform<Derived>::generate_grid(Options& opts) {
    auto& S = self();
    
    std::random_device rd;
    std::mt19937 gen(rd());

    // [TODO] Вынести в сущность настроек.
    double attempts_count = 5; // Количество попыток разместить точку внутри квадрата без коллизии, если включена проверка коллизии.

    // Функция для генерации точки внутри заданной области с проверкой коллизии по необходимости.
    auto generate_point = [&S, &gen, &attempts_count, check_collision = opts.check_collision](const Point& min, const Point& max) {
        struct Result {
            Point p;
            bool is_generated{false};
        };

        if (!check_collision) { // Генерация точки без проверки коллизии.
            Point p (
                std::uniform_real_distribution(min.x, max.x)(gen),
                std::uniform_real_distribution(min.y, max.y)(gen)
            );
            return Result{p, true};
        };
        // Иначе точка генерируется с проверкой коллизии.
        
        for (int i = 0; i < attempts_count; ++i) {
            Point p (
                std::uniform_real_distribution(min.x, max.x)(gen),
                std::uniform_real_distribution(min.y, max.y)(gen)
            );
            if (S.collision(p)) continue; // Пропускаем точку, если обнаружена коллизия.
            
            p.is_checked_collsn = true;
            return Result{p, true};
        }
        
        return Result{.is_generated = false};
    };

    Graph result;

    // Количество прямоугольников вдоль осей для размещения точек.
    int32_t cells_per_side = std::sqrt(opts.nodes_count);

    // Остаток точек, которые будут распределены случайным образом, если вдоль оси не помещается целое число квадратов.
    int32_t remain_points_count = std::max(opts.nodes_count - cells_per_side * cells_per_side, 0);

    // Размеры ячейки.
    double dx = (S.corner_max.x - S.corner_min.x) / cells_per_side;
    double dy = (S.corner_max.y - S.corner_min.y) / cells_per_side;

    // Границы ячейки.
    double y_l{-dy}, y_r{0};
    double x_l{-dx}, x_r{0};

    // Распределение точек по ячейкам.
    for (int64_t i = 0; i < cells_per_side; ++i) { // Цикл вдоль оси Y.
        y_l += dy; y_r += dy;
        x_l = -dx; x_r = 0;
        for (int64_t j = 0; j < cells_per_side; ++j) { // Цикл вдоль оси X.
            x_l += dx; x_r += dx;

            auto [p, is_generated] = generate_point({x_l,y_l}, {x_r,y_r});
            if (!is_generated) continue; // Пропускаем точку, если её не удалось сгенерировать в данном квадрате.

            result.add(p);
        }
    }

    // Распределение оставшихся точек.
    for (int32_t i = 0; i < remain_points_count; ++i) {
        auto [p, is_generated] = generate_point(S.corner_min, S.corner_max);
        if (!is_generated) { --i; continue; } // Если точку не удалось сгенерировать, то пробуем ещё раз.

        result.add(p);
    }
    
    if (opts.connect) {
        result = S.link_grid({}, result, S.stgs.nearest_count, opts.check_collision);
    }
    
    S.visual.picture({S.task, {.enhance = result}, "uniform_grid"});

    return result;
}
