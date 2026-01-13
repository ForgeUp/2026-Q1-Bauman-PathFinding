#pragma once

#include <vector>

#include "types/Point.hpp"
#include "types/Segment.hpp"


struct Solution {
    std::vector<Point> path;
    std::vector<Segment> grid;
    std::vector<Segment> examined_roads;
    std::vector<Segment> invalid_roads;
    bool is_fail{false};
};
