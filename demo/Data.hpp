#pragma once

#include <string>

#include "types/Task.hpp"
#include "types/SolverSettings.hpp"


namespace demo {

struct Data {
    Task task;
    SolverSettings stgs;

    VisualizerService& srv_visual;

    std::string folder_mark;
    int32_t runs_per_area{1};
};

}
