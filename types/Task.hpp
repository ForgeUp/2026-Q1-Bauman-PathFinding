#pragma once

#include "types/Point.hpp"
#include "types/Area.hpp"


struct Task {
    Point start{};
    Point end{};

    Area area{};
};

Task no_rocks(const Task& task) {
    return {
        .start = task.start,
        .end   = task.end,
        .area  = Area{
            .x_min = task.area.x_min,
            .x_max = task.area.x_max,
            .y_min = task.area.y_min,
            .y_max = task.area.y_max,
        }
    };
}
