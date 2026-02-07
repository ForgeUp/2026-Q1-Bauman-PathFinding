#pragma once

#include <vector>

#include "types/Point.hpp"
#include "types/Graph.hpp"

#include "qtree/qtree.hpp"


struct Solution {
    Graph path;
    Graph grid;
    Graph enhance;
    Graph examined;
    Graph invalid;
    Graph invalid_all;
    Qtree qtree;
    bool is_fail{false};
};
