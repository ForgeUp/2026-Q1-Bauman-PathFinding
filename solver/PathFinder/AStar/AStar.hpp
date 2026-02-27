#pragma once


namespace PathFinder {

template <typename Derived>
class AStar {
protected:
    Derived& self() { return static_cast<Derived&>(*this); }

    void find_path();
};

}


#include "find_path.inl"
