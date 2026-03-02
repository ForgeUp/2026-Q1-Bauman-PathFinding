#pragma once

#include "qtree/Qtree.hpp"
using QTreeDS = Qtree;

#include "types/Point.hpp"


namespace CollisionChecker {

template <typename Derived>
class Qtree {
public:
    Derived& self() { return static_cast<Derived&>(*this); }

    bool check_points_collision();
    bool check_edges_collision();

    bool collision(const Point& p);

private:
    bool is_init{false};
    QTreeDS qtree;

private:
    void build_qtree();
};

}


#include "build_qtree.inl"
#include "check_points_collision.inl"
#include "check_edges_collision.inl"
#include "collision.inl"
