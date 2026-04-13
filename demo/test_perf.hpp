#pragma once

#include <algorithm>
#include <ranges>

#include "demo/Data.hpp"
#include "demo/tester.hpp"
#include "demo/mean_metric.hpp"
#include "demo/plot_time.hpp"
#include "demo/plot_collision.hpp"


namespace demo {

template <typename Solver>
void test_perf(Data& data, std::string name = "") {
    GeneratorConfig cfg = {
        .x_min = 0,
        .x_max = 100,
        .y_min = 0,
        .y_max = 100,

        .generate_rand_seed = true,
        // .seed = 42,

        .rand_home_points = false,
        .start = Point(10.0, 10.0),
        .end   = Point(90.0, 90.0),
        
        .polygon_density = 0.05,
        .polygon_max_vertices = 4,
        .polygon_min_radius = 1,
        .polygon_max_radius = 3,
        .border_margin = 5,
    };

    std::vector<std::pair<double,Metric>> result;

    for (auto denst : {0.01, 0.02, 0.03, 0.04, 0.05}) {
        cfg.polygon_density = denst;

        std::vector<Metric> metrics; // Вектор метрик за все запуски.
        for (int32_t r = 0; r < data.runs_per_area; ++r) {
            data.task = taskgen::task(cfg);
        
            data.folder_mark = std::to_string(cfg.polygon_density) + " "+ std::to_string(r+1);
            std::replace(data.folder_mark.begin(), data.folder_mark.end(), '.', '_');
            
            Metric metric = demo::tester<Solver>(data);

            metrics.push_back(std::move(metric));
        }

        result.push_back({denst, demo::mean_metric(metrics)});
    }

    demo::plot_time(result, name);
    demo::plot_collision(result, name);
}

}
