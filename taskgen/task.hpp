#pragma once

#include <fstream>
#include <sstream>
#include <random>
#include <cmath>
#include <numbers>
using std::numbers::pi;

#include "types/Rock.hpp"
#include "types/Area.hpp"
#include "types/Task.hpp"
#include "types/GeneratorConfig.hpp"

#include "geometry/is_inside.hpp"


namespace taskgen {

Rock polygon(const GeneratorConfig& cfg, std::mt19937& gen) {
    Rock rock;

    int vertices  = std::uniform_int_distribution(3, cfg.polygon_max_vertices)(gen);
    double radius = std::uniform_real_distribution(cfg.polygon_min_radius, cfg.polygon_max_radius)(gen);

    Point center{
        std::uniform_real_distribution(cfg.x_min, cfg.x_max)(gen),
        std::uniform_real_distribution(cfg.y_min, cfg.y_max)(gen)
    };

    double angleStep = 2 * pi / vertices;

    for (int i = 0; i < vertices; ++i) {
        double angle = i * angleStep + std::uniform_real_distribution(-0.3, 0.3)(gen);
        rock.points.push_back({
            center.x + radius * std::cos(angle),
            center.y + radius * std::sin(angle)
        });
    }
    return rock;
}

// Функция для создания n точек вокруг заданной точки m на расстоянии r.
std::vector<Point> circle_points(const Point& m, double r, int32_t n) {
    if (n < 1) return {};
    std::vector<Point> result;
    double angleStep = 2 * pi / n;
    for (int32_t i = 0; i < n; ++i) {
        double angle = i * angleStep;
        result.push_back({
            m.x + r * std::cos(angle),
            m.y + r * std::sin(angle)
        });
    }
    return result;
}

Task task(GeneratorConfig& cfg) {
    if (cfg.generate_rand_seed) cfg.seed = std::random_device()();
    std::mt19937 gen(cfg.seed);

    Task task;

    task.area.x_min = cfg.x_min;
    task.area.x_max = cfg.x_max;
    task.area.y_min = cfg.y_min;
    task.area.y_max = cfg.y_max;

    double areaSize =
        (cfg.x_max - cfg.x_min) *
        (cfg.y_max - cfg.y_min);

    int polygonCount = static_cast<int>(areaSize * cfg.polygon_density);

    if (cfg.rand_home_points) cfg.start = {
        std::uniform_real_distribution(
            cfg.x_min + cfg.border_margin,
            cfg.x_max - cfg.border_margin)(gen),
        std::uniform_real_distribution(
            cfg.y_min + cfg.border_margin,
            cfg.y_max - cfg.border_margin)(gen)
    };

    if (cfg.rand_home_points) cfg.end = {
        std::uniform_real_distribution(
            cfg.x_min + cfg.border_margin,
            cfg.x_max - cfg.border_margin)(gen),
        std::uniform_real_distribution(
            cfg.y_min + cfg.border_margin,
            cfg.y_max - cfg.border_margin)(gen)
    };

    task.start = cfg.start;
    task.end   = cfg.end;

    std::vector<Point> start_circle = circle_points(task.start, 0.5, 8);
    std::vector<Point> end_circle   = circle_points(task.end,   0.5, 8);

    auto valid_rock = [](const Point& p, const std::vector<Point>& circle, const Rock& r) {
        return !geometry::is_inside(p, r) && !geometry::is_inside(circle, r);
    };

    // Генерация препятствий.
    for (int i = 0; i < polygonCount; ++i) {
        auto r = polygon(cfg, gen);

        // Проверка, что начальная и конечная точки НЕ находятся внутри многоугольника.
        if (!valid_rock(task.start, start_circle, r) || !valid_rock(task.end, end_circle, r)) {
            i--;
            continue;
        }

        task.area.rocks.push_back(r);
    }

    return task;
}

}
