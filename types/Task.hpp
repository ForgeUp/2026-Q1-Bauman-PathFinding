#pragma once

#include "types/Point.hpp"
#include "types/Area.hpp"


struct Task {
    Point start{};
    Point end{};

    Area area{};
};
