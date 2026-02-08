#pragma once

#include "qtree.hpp"


void Qtree::expand(Box* box) {
    auto& b = *box;
    if (!b.is_leaf) return;

    b.q1 = create_box({b.x_mid, b.y_mid}, {b.x_max, b.y_max});
    b.q2 = create_box({b.x_min, b.y_mid}, {b.x_mid, b.y_max});
    b.q3 = create_box({b.x_min, b.y_min}, {b.x_mid, b.y_mid});
    b.q4 = create_box({b.x_mid, b.y_min}, {b.x_max, b.y_mid});
    
    if (b.type == Type::Busy) b.q1->type = b.q2->type = b.q3->type = b.q4->type = Type::Busy;

    b.is_leaf = false;
    b.type = Type::Inner;
}
