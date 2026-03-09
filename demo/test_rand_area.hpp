#pragma once

#include "demo/Data.hpp"
#include "demo/tester.hpp"

#include "types/GeneratorConfig.hpp"
#include "types/SolverSettings.hpp"

#include "taskgen/task.hpp"


namespace demo {

template <typename Solver>
void test_rand_area(Data& data) {
    GeneratorConfig cfg = {
        .x_min = 0,
        .x_max = 100,
        .y_min = 0,
        .y_max = 100,

        .generate_rand_seed = false,
        .seed = 42,

        .rand_home_points = false,
        .start = Point(10.0, 10.0),
        .end   = Point(90.0, 90.0),
        
        .polygon_density = 0.05,
        .polygon_max_vertices = 4,
        .polygon_min_radius = 1,
        .polygon_max_radius = 3,
        .border_margin = 5,
    };
    data.task = taskgen::task(cfg);
    
    demo::tester<Solver>(data);
}

}
