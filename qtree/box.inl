#pragma once

#include <stack>

#include "qtree.hpp"


Qtree::Box* Qtree::create_box(const Point& min, const Point& max) {
    return new Box{
        .x_min = min.x,
        .x_mid = (min.x + max.x) / 2,
        .x_max = max.x,
        .y_min = min.y,
        .y_mid = (min.y + max.y) / 2,
        .y_max = max.y
    };
}

Qtree::Box* Qtree::copy_box(const Box* other) {
    if (!other) return nullptr;
    Box* b = new Box(*other);
    b->q1 = copy_box(other->q1);
    b->q2 = copy_box(other->q2);
    b->q3 = copy_box(other->q3);
    b->q4 = copy_box(other->q4);
    return b;
}

void Qtree::clear_box(Box* b) {
    if (!b) return;
    clear_box(b->q1);
    clear_box(b->q2);
    clear_box(b->q3);
    clear_box(b->q4);
    delete b;
}

void Qtree::clear() {
    std::stack<Box*> boxs;
    if (root != nullptr) boxs.push(root);
    while (boxs.size() > 0) {
        Box* curr = boxs.top();
        boxs.pop();

        if (curr->q1 != nullptr) boxs.push(curr->q1);
        if (curr->q2 != nullptr) boxs.push(curr->q2);
        if (curr->q3 != nullptr) boxs.push(curr->q3);
        if (curr->q4 != nullptr) boxs.push(curr->q4);

        delete curr;
    }
}
