#pragma once

#include "qtree/Qtree.hpp"
using QTreeDS = Qtree;

#include "types/Point.hpp"


namespace CollisionChecker {

template <typename Derived>
class Qtree {
public:
    Derived& self() { return static_cast<Derived&>(*this); }

    void init();

    bool check_points_collision();
    bool check_edges_collision();

    bool collision(const Point& p);

private:
    bool is_init{false};
    QTreeDS qtree;
};

}


#include "init.inl"
#include "check_points_collision.inl"
#include "check_edges_collision.inl"
#include "collision.inl"
