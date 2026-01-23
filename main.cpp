
#include <format>
#include <string>

#include "types/Task.hpp"

#include "generate_task.hpp"
#include "solve.hpp"
#include "to_file.hpp"


int main() {
    GeneratorConfig cfg = {
        .x_min = 0,
        .x_max = 100,
        .y_min = 0,
        .y_max = 100,
        
        .polygon_density = 0.005,
        .polygon_max_vertices = 4,
        .polygon_min_radius = 2,
        .polygon_max_radius = 6,
        .border_margin = 5,
    };
    auto task = generate_task(cfg);
    
    SolverSettings stgs = {
        .squares_per_side = 10,
        .attempts_count = 5
    };
    auto sln = solve(task, stgs);

    to_file("data/area.gp", std::format("x_min={}\nx_max={}\ny_min={}\ny_max={}\n", task.area.x_min, task.area.x_max, task.area.y_min, task.area.y_max));
    to_file("data/polygons.txt", task.area.rocks);
    to_file("data/grid.txt", sln.grid);
    to_file("data/examined_roads.txt", sln.examined_roads);
    to_file("data/path.txt", sln.path);
    to_file("data/invalid_roads.txt", sln.invalid_roads);

    return 0;
}
