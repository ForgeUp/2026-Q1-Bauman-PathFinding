
#include <chrono>

#include "taskgen/task.hpp"
#include "solver/Solver/ClusterOffset.hpp"


int main() {
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
        .enhance_rand_nodes_count = 100,
        .enhance_seed_nodes_count = 0,
        .enhance_attempts_limit = 10
    };
    
    
    for (int32_t i = 0; i < 10; ++i) {
        auto start = std::chrono::steady_clock::now();
        
        auto solver = solver::ClusterOffset(task, stgs);
        auto sln = solver.run();
        
        auto end = std::chrono::steady_clock::now();
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Total time consume: " << duration_ms << '\n';

        for (const auto& [name, stamp] : sln.metric) {
            std::cout << '\t' << name << ' ' << std::chrono::duration_cast<std::chrono::milliseconds>(stamp.acc) << '\n';
        }
    }

    return 0;
}
