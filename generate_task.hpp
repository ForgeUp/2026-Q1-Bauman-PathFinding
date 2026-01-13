#pragma once

#include <fstream>
#include <sstream>
#include <random>
#include <cmath>
#include <numbers>
using std::numbers::pi;

#include "types/Rock.hpp"
#include "types/Task.hpp"

#include "random/from_range.hpp"


struct GeneratorConfig {
    double x_min{0};
    double x_max{100};
    double y_min{0};
    double y_max{100};

    double polygon_density{0.05};
    int polygon_max_vertices{3};
    double polygon_min_radius{2};
    double polygon_max_radius{8};

    double border_margin{5};
};

Rock generate_polygon(const GeneratorConfig& cfg, std::mt19937& gen) {
    Rock rock;

    int vertices  = random::from_range(3, cfg.polygon_max_vertices, gen);
    double radius = random::from_range(cfg.polygon_min_radius, cfg.polygon_max_radius, gen);

    Point center{
        random::from_range(cfg.x_min, cfg.x_max, gen),
        random::from_range(cfg.y_min, cfg.y_max, gen)
    };

    double angleStep = 2 * pi / vertices;

    for (int i = 0; i < vertices; ++i) {
        double angle = i * angleStep + random::from_range(-0.3, 0.3, gen);
        rock.points.push_back({
            center.x + radius * std::cos(angle),
            center.y + radius * std::sin(angle)
        });
    }
    return rock;
}

Task generate_task(const GeneratorConfig& cfg) {
    std::random_device rd;
    std::mt19937 gen(rd());

    Task task;

    task.area.x_min = cfg.x_min;
    task.area.x_max = cfg.x_max;
    task.area.y_min = cfg.y_min;
    task.area.y_max = cfg.y_max;

    double areaSize =
        (cfg.x_max - cfg.x_min) *
        (cfg.y_max - cfg.y_min);

    int polygonCount = static_cast<int>(areaSize * cfg.polygon_density);

    task.start = {
        random::from_range(cfg.x_min + cfg.border_margin,
                     cfg.x_max - cfg.border_margin, gen),
        random::from_range(cfg.y_min + cfg.border_margin,
                     cfg.y_max - cfg.border_margin, gen)
    };

    task.end = {
        random::from_range(cfg.x_min + cfg.border_margin,
                     cfg.x_max - cfg.border_margin, gen),
        random::from_range(cfg.y_min + cfg.border_margin,
                     cfg.y_max - cfg.border_margin, gen)
    };

    for (int i = 0; i < polygonCount; ++i) {
        task.area.rocks.push_back(generate_polygon(cfg, gen));
    }

    return task;
}

