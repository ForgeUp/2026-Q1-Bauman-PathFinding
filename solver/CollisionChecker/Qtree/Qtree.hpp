#pragma once

#include "qtree/Qtree.hpp"
using QTreeDS = Qtree;


namespace CollisionChecker {

template <typename Derived>
class Qtree {
protected:
    Derived& self() { return static_cast<Derived&>(*this); }

    bool check_points_collision();
    bool check_edges_collision();

protected:
    bool is_init{false};
    QTreeDS qtree;

private:
    void build_qtree();
};

}


#include "build_qtree.inl"
#include "check_points_collision.inl"
#include "check_edges_collision.inl"
