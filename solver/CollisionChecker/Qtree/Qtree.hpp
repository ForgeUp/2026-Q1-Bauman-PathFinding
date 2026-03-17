#pragma once

#include <map>

#include "qtree/Qtree.hpp"
using QTreeDS = Qtree;

#include "types/Point.hpp"
#include "types/Segment.hpp"


namespace CollisionChecker {

template <typename Derived>
class Qtree {
public:
    Derived& self() { return static_cast<Derived&>(*this); }

    void init();

    bool check_points_collision();
    bool check_edges_collision();

    bool collision(const Point& p);
    bool collision(const Segment& s);

private:
    bool is_init{false};
    QTreeDS qtree;

    std::map<Segment,bool> edge_cache;
};

}


#include "init.inl"
#include "check_points_collision.inl"
#include "check_edges_collision.inl"
#include "collision.inl"
