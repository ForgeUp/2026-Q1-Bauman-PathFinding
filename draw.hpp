#pragma once

#include <format>
#include <string>
#include <cstdlib>
#include <filesystem>

#include "types/Task.hpp"
#include "types/Solution.hpp"

#include "to_file.hpp"


void draw(const Task& task, const Solution& sln) {
    static int32_t i = 0;
    static bool clear_dir = false;
    constexpr const char* foldername{"data/result"};
    if (!clear_dir) {
        std::filesystem::remove_all(foldername);
        std::filesystem::create_directories(foldername);
        clear_dir = true;
    }

    to_file("data/area.gp", std::format("x_min={}\nx_max={}\ny_min={}\ny_max={}\nx_start={}\ny_start={}\nx_end={}\ny_end={}", task.area.x_min, task.area.x_max, task.area.y_min, task.area.y_max, task.start.x, task.start.y, task.end.x, task.end.y));
    to_file("data/polygons.txt", task.area.rocks);
    to_file("data/grid.txt", sln.grid);
    to_file("data/examined_roads.txt", sln.examined);
    to_file("data/path.txt", sln.path);
    to_file("data/invalid_roads.txt", sln.invalid);
    
    std::string cmd = std::format("gnuplot -e \"filename='{}/{}.png'\" draw.gp", foldername, i++);
    system(cmd.c_str());
}
