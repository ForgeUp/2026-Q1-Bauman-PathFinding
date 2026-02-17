#pragma once

#include <vector>

#include "qtree.hpp"

#include "types/Segment.hpp"


// Метод для растеризации квадрантов вдоль отрезка. Предусмотрена обрезка отрезков, выходящих за пределы области дерева.
void Qtree::add(const Segment& s, SegIdx idx, ObstID obst) {
    std::vector<Box*> trail = trace<true>(s);
    for (const auto& b : trail) {
        b->type = b->type == Type::Busy ? Type::Busy : Type::Mix; 
        b->collisions[obst].insert(idx);
    }
}
