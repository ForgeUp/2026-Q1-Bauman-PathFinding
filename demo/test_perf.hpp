#pragma once

#include <algorithm>

#include "demo/Data.hpp"
#include "demo/tester.hpp"


namespace demo {

template <typename Solver>
void test_perf(Data& data) {
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

    std::vector<std::pair<double,Metric>> result;

    for (auto denst : {0.01, 0.02, 0.03, 0.04, 0.05}) {
        cfg.polygon_density = denst;
        data.task = taskgen::task(cfg);
        
        data.folder_mark = std::to_string(cfg.polygon_density);
        std::replace(data.folder_mark.begin(), data.folder_mark.end(), '.', '_');
        
        std::string output_folder = std::format("{:%Y-%m-%d %H-%M-%S} [{}]", 
            std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()), 
            data.folder_mark
        );
        Services srvs = {
            .visual = VisualizerAdapter(data.srv_visual, output_folder) 
        };

        Logger::log("Run {}", data.folder_mark);

        auto solver = Solver(data.task, data.stgs, srvs);
        auto sln = solver.run();

        result.push_back({denst, sln.metric});
    }
    
    std::vector<std::pair<double,int64_t>> totals;
    for (const auto& [denst, metric] : result) {
        totals.push_back({
            denst,
            std::chrono::duration_cast<std::chrono::milliseconds>(metric.total).count()
        });
    }

    std::filesystem::create_directories("result/tmp");
    to_file("result/tmp/perf.txt", totals);

    gnuplot::exec({
        .script_name     = "perf.gp",
        .script_dir_path = "gnuplot",
        .filename        = "perf",
        .output_dir_path = "result",
        .data_dir_path   = "result/tmp"
    });
}

}
