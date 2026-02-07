#pragma once

#include "qtree.hpp"

#include "types/Point.hpp"


Qtree::Box* Qtree::locate(const Point& p, Box* from) {
    if (!from->is_inside(p)) return nullptr;
    Box* curr = from;
    while (!curr->is_leaf) {
        if (curr->is_inside_q1(p)) curr = curr->q1;
        else if (curr->is_inside_q2(p)) curr = curr->q2;
        else if (curr->is_inside_q3(p)) curr = curr->q3;
        else if (curr->is_inside_q4(p)) curr = curr->q4;
    }
    
    return curr;
}

Qtree::Box* Qtree::locate(const Point& p) {
    return locate(p, root);
}
