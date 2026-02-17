
#include "taskgen/task.hpp"
#include "solver/Solver/Qtree.hpp"


int main() {
    GeneratorConfig cfg = {
        .x_min = 0,
        .x_max = 100,
        .y_min = 0,
        .y_max = 100,

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
        .connection_radius = 20,
        .enhance_rand_nodes_count = 100,
        .enhance_seed_nodes_count = 0,
        .enhance_attempts_limit = 10
    };
    auto solver = solver::Qtree(task, stgs);

    auto sln = solver.run();

    return 0;
}
