#pragma once

#include <vector>

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "types/Metric.hpp"

#include "qtree/qtree.hpp"


struct Solution {
    Graph path;
    Graph grid;
    Graph enhance;
    Graph examined;
    Graph invalid;
    Graph invalid_all;
    Qtree qtree;
    Metric metric;
    bool is_fail{false};
};
