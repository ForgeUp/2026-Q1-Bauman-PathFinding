#pragma once

#include <map>

#include "solver/CollisionChecker/Base/Base.hpp"

#include "types/Point.hpp"
#include "types/Segment.hpp"


namespace CollisionChecker {

template <typename Derived>
class Naive : public CollisionChecker::Base<Derived> {
public:
    Derived& self() { return static_cast<Derived&>(*this); }

    bool collision(const Point& p);
    bool collision(const Segment& s);

private:
    std::map<Segment,bool> edge_cache;
};

}


#include "collision.inl"
