#pragma once


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
