#pragma once

#include "solver/VarsBase.hpp"

#include "types/Task.hpp"
#include "types/SolverSettings.hpp"


namespace GridEnhancer {

class Article2001 : virtual public VarsBase {
public:
    Article2001(const Task& task_, const SolverSettings& stgs_) : VarsBase(task_, stgs_) {}
    
protected:
    bool enhance_graph();
};

};


#include "enhance_graph.inl"
