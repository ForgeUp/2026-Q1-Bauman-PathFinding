
#include "qtree.hpp"
#include "types/Point.hpp"

#include "taskgen/task.hpp"
#include "draw.hpp"


void test1() {
    Task task = {
        .area = Area{
            .x_min = 0,
            .x_max = 100,
            .y_min = 0,
            .y_max = 100,
        }
    };

    Qtree qtree(Point(task.area.x_min,task.area.y_min), Point(task.area.x_max,task.area.y_max));
    qtree.add(Rock{{Point(20, 70), Point(80, 70), Point(50, 20)}});
    
    draw(task, {.qtree = qtree}, "qtree");
}

void test2() {
    Task task = {
        .area = Area{
            .x_min = 0,
            .x_max = 100,
            .y_min = 0,
            .y_max = 100,
        }
    };

    Qtree qtree(Point(task.area.x_min,task.area.y_min), Point(task.area.x_max,task.area.y_max));
    qtree.add(Rock{{Point(20, 20), Point(80, 80), Point(80, 20)}});
    
    draw(task, {.qtree = qtree}, "qtree");
}

void test3() {
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

    Qtree qtree(Point(cfg.x_min,cfg.y_min), Point(cfg.x_max,cfg.y_max));
    for (const auto& r : task.area.rocks) {
        qtree.add(r);
    }

    draw(task, {.qtree = qtree}, "qtree");
}

int main() {
    std::filesystem::path project_root = "D:\\1 Reps\\2026-Q1-Bauman-PathFinding";
    std::filesystem::current_path(project_root);

    test1();
    test2();
    test3();
}
