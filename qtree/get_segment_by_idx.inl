#pragma once 

#include "qtree.hpp"


Segment Qtree::get_segment_by_idx(ObstID obst, SegIdx idx) {
    return obst->edges().begin()[idx];
}
