#pragma once

#include "types/Point.hpp"
#include "types/Segment.hpp"


namespace CollisionChecker {

template <typename Derived>
class Base {
public:
    Derived& self() { return static_cast<Derived&>(*this); }

    bool check_points_collision();
    bool check_edges_collision();

    bool check_path_collision();
};

}


#include "check_points_collision.inl"
#include "check_path_collision.inl"
#include "check_edges_collision.inl"
