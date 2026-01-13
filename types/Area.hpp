#pragma once

#include <vector>

#include "types/Rock.hpp"


struct Area {
    double x_min{0};
    double x_max{0};

    double y_min{0};
    double y_max{0};
    
    std::vector<Rock> rocks;
};
