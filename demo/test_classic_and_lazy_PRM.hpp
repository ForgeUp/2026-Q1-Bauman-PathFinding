#pragma once

#include "solver/Solver/Uniform.hpp"

#include "types/GeneratorConfig.hpp"
#include "types/SolverSettings.hpp"

#include "taskgen/task.hpp"

#include "demo/Data.hpp"
#include "demo/examples.hpp"
#include "demo/tester.hpp"
#include "demo/test_perf.hpp"


namespace demo {

// Сравнение стандратного PRM и ленивого PRM.
template <typename Solver>
void test_classic_and_lazy_PRM(Data& data){
    for (const auto& [title, task] : Examples{}.tasks) {
        data.task = task;

        data.folder_mark = "classic " + title;
        data.stgs.lazy   = false;
        demo::tester<Solver>(data);

        data.folder_mark = "lazy " + title;
        data.stgs.lazy   = true;
        demo::tester<Solver>(data);
    }

    GeneratorConfig cfg = {
        .x_min = 0,
        .x_max = 100,
        .y_min = 0,
        .y_max = 100,

        .generate_rand_seed = true,
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

    data.folder_mark = "classic random";
    data.stgs.lazy   = false;
    demo::tester<Solver>(data);
    demo::test_perf<Solver>(data, "classic");

    data.folder_mark = "lazy random";
    data.stgs.lazy   = true;
    demo::tester<Solver>(data);
    demo::test_perf<Solver>(data, "lazy");
}

}
