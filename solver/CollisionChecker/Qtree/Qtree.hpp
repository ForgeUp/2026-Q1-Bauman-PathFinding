#pragma once

#include <map>

#include "solver/CollisionChecker/Base/Base.hpp"

#include "qtree/qtree.hpp"
using QTreeDS = Qtree;

#include "types/Point.hpp"
#include "types/Segment.hpp"


namespace CollisionChecker {

template <typename Derived>
class Qtree : public CollisionChecker::Base<Derived> {
public:
    Derived& self() { return static_cast<Derived&>(*this); }

    void init();

    bool collision(const Point& p);
    bool collision(const Segment& s);

private:
    bool is_init{false};
    QTreeDS qtree;

    std::map<Segment,bool> edge_cache;
};

}


#include "init.inl"
#include "collision.inl"
