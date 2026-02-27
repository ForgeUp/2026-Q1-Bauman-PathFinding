#pragma once


namespace GridEnhancer {

template <typename Derived>
class Article2001 { 
protected:
    Derived& self() { return static_cast<Derived&>(*this); }

    bool enhance_graph();
};

}


#include "enhance_graph.inl"
