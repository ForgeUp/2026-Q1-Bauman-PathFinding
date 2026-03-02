
#include <algorithm>
#include <chrono>
#include <format>

#include "types/Services.hpp"

#include "taskgen/task.hpp"
#include "solver/Solver/Article2001.hpp"

#include "draw/VisualizerService.hpp"

#include "timer/Timer.hpp"


int main() {
    VisualizerService srv_visual("result");

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
    auto task = taskgen::task(cfg);
    
    SolverSettings stgs = {
        .initial_nodes_count = 100,
        .connection_radius = 10,
        .nearest_count = 6,
        .enhance_rand_nodes_count = 100,
        .enhance_seed_nodes_count = 0,
        .bridge_standard_deviation = 2,
        .enhance_attempts_limit = 10
    };
    
    int32_t runs_count = 10;
    for (int32_t i = 0; i < runs_count; ++i) {
        std::string output_folder = std::format("{:%Y-%m-%d %H-%M-%S} [{:2}]", std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()), i+1);
        Services srvs = {
            .visual = VisualizerAdapter(srv_visual, output_folder) 
        };
        Timer timer;
        
        auto solver = solver::Article2001(task, stgs, srvs);
        auto sln = solver.run();
        
        auto total = timer.tick();
        std::cout << std::format("[{:2}/{}] Total time consume: {}", i+1, runs_count, total) << '\n';

        std::vector<std::pair<std::string,Metric::Stamp>> metric;
        for (const auto& [name, stamp] : sln.metric) {
            metric.emplace_back(name, stamp);
        }
        std::sort(metric.begin(), metric.end(), [](auto& l, auto& r) {
            return l.second.acc > r.second.acc;
        });
        for (const auto& [name, stamp] : metric) {
            std::cout << std::format("\t{:<30} | {:6} | {:10} | {:6.2f}%", 
                name,
                stamp.counter,
                std::chrono::duration_cast<std::chrono::milliseconds>(stamp.acc), 
                100.0 * stamp.acc / total
            ) << '\n';
        }
    }

    return 0;
}
