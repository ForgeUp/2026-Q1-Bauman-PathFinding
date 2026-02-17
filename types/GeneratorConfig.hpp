#pragma once

#include <random>

#include "types/Point.hpp"


struct GeneratorConfig {
    double x_min{0};
    double x_max{100};
    double y_min{0};
    double y_max{100};

    bool generate_rand_seed{true};
    std::random_device::result_type seed{0};

    bool rand_home_points{false};
    Point start = Point(10.0, 10.0);
    Point end   = Point(90.0, 90.0);

    double polygon_density{0.05};
    int polygon_max_vertices{3};
    double polygon_min_radius{2};
    double polygon_max_radius{8};

    double border_margin{5};
};
