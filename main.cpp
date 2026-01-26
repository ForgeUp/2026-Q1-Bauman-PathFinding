
#include <format>
#include <string>

#include "taskgen/task.hpp"
#include "solve.hpp"
#include "to_file.hpp"


int main() {
    GeneratorConfig cfg = {
        .x_min = 0,
        .x_max = 100,
        .y_min = 0,
        .y_max = 100,
        
        .polygon_density = 0.05,
        .polygon_max_vertices = 4,
        .polygon_min_radius = 1,
        .polygon_max_radius = 3,
        .border_margin = 5,
    };
    auto task = taskgen::task(cfg);
    task.start = Point(10,10);
    task.end   = Point(90,90);
    
    SolverSettings stgs = {
        .initial_nodes_count = 500,
        .connection_radius = 10,
        .enhance_rand_nodes_count = 50,
        .enhance_seed_nodes_count = 50,
        .enhance_attempts_limit = 5
    };
    auto sln = solve(task, stgs);

    to_file("data/area.gp", std::format("x_min={}\nx_max={}\ny_min={}\ny_max={}\nx_start={}\ny_start={}\nx_end={}\ny_end={}", task.area.x_min, task.area.x_max, task.area.y_min, task.area.y_max, task.start.x, task.start.y, task.end.x, task.end.y));
    to_file("data/polygons.txt", task.area.rocks);
    to_file("data/grid.txt", sln.grid);
    to_file("data/examined_roads.txt", sln.examined);
    to_file("data/path.txt", sln.path);
    to_file("data/invalid_roads.txt", sln.invalid);

    return 0;
}
