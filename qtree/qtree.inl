#pragma once

#include "qtree.hpp"


Qtree& Qtree::operator=(const Qtree& other) {
    if (this == &other) return *this;
    clear();

    root = copy_box(other.root);
    min_box_size_point = other.min_box_size_point;
    
    min = other.min;
    max = other.max;

    obstacles = other.obstacles;
    
    return *this;
}
