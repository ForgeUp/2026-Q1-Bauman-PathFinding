#pragma once

#include <stack>

#include "qtree.hpp"

#include "types/Point.hpp"


std::ostream& operator<<(std::ostream& os, const Qtree& qtree) {
    std::stack<Qtree::Box*> bs;
    bs.push(qtree.root);
    while (bs.size() > 0) {
        Qtree::Box* b = bs.top();
        bs.pop();

        if (b == nullptr) continue;
        if (b->is_leaf) continue;

        os << Segment{Point(b->x_min,b->y_mid), Point(b->x_max,b->y_mid)}  // Горизонтальная линия.
           << Segment{Point(b->x_mid,b->y_max), Point(b->x_mid,b->y_min)}; // Вертикальная линия.

        bs.push(b->q1);
        bs.push(b->q2);
        bs.push(b->q3);
        bs.push(b->q4);
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Qtree::Colors& colors) {
    std::stack<Qtree::Box*> bs;
    bs.push(colors.qtree.root);
    while (bs.size() > 0) {
        Qtree::Box* b = bs.top();
        bs.pop();

        if (b == nullptr) continue;
        if (b->is_leaf) {
            if (b->type != colors.select) continue;

            os << b->x_max << ' ' << b->y_max << '\n'
               << b->x_min << ' ' << b->y_max << '\n'
               << b->x_min << ' ' << b->y_min << '\n'
               << b->x_max << ' ' << b->y_min << '\n'
               << '\n';
                
            continue;
        }

        bs.push(b->q1);
        bs.push(b->q2);
        bs.push(b->q3);
        bs.push(b->q4);
    }
    return os;
}
