#pragma once

#include "Solver/Solver/Uniform.hpp"

#include "demo/Data.hpp"
#include "demo/examples.hpp"
#include "demo/tester.hpp"


namespace demo {

// Сравнение стандратного PRM и ленивого PRM.
void test_classic_and_lazy_PRM(VisualizerService& srv_visual) {
    SolverSettings stgs = {
        .lazy = true,
        .initial_nodes_count = 500,
        .nearest_count = 6,
        .bridge_standard_deviation = 2,
        .gauss_standard_deviation = 1,
        .enhance_attempts_limit = 10
    };

    demo::Data data {
        .stgs = stgs,
        .srv_visual = srv_visual,
    };

    using Solver = solver::Uniform;

    for (const auto& [title, task] : Examples{}.tasks) {
        data.task = task;

        data.folder_mark = "classic " + title;
        data.stgs.lazy   = false;
        demo::tester<Solver>(data);

        data.folder_mark = "lazy " + title;
        data.stgs.lazy   = true;
        demo::tester<Solver>(data);
    }
}

}
