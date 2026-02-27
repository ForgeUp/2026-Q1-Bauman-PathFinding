#pragma once


namespace InitialGrider {

template <typename Derived>
class Nearest {
protected:
    Derived& self() { return static_cast<Derived&>(*this); }

    void generate_initial_grid();
};

}


#include "generate_initial_grid.inl"
