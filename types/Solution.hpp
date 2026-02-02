#pragma once

#include <vector>

#include "types/Point.hpp"
#include "types/Graph.hpp"


struct Solution {
    Graph path;
    Graph grid;
    Graph examined;
    Graph invalid;
    bool is_fail{false};
};
