#pragma once

#include <map>

#include "types/Point.hpp"
#include "types/Segment.hpp"


namespace CollisionChecker {

template <typename Derived>
class Naive {
public:
    Derived& self() { return static_cast<Derived&>(*this); }

    bool check_points_collision();
    bool check_edges_collision();

    bool collision(const Point& p);
    bool collision(const Segment& s);

private:
    std::map<Segment,bool> edge_cache;
};

}


#include "check_points_collision.inl"
#include "check_edges_collision.inl"
#include "collision.inl"
