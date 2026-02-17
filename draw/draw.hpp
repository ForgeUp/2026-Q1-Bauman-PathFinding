#pragma once

#include <format>
#include <string>
#include <cstdlib>
#include <filesystem>

#include "types/Task.hpp"
#include "types/Solution.hpp"

#include "to_file.hpp"


void draw(const Task& task, const Solution& sln, const std::string& name = "") {
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
    to_file("data/enhance.txt", sln.enhance.verts);
    to_file("data/examined.txt", sln.examined);
    to_file("data/invalid.txt", sln.invalid);
    to_file("data/invalid_all.txt", sln.invalid_all);
    to_file("data/path.txt", sln.path);
    to_file("data/qtree.txt", sln.qtree);
    to_file("data/qtree_free.txt", sln.qtree.colors(Qtree::Type::Free));
    to_file("data/qtree_mix.txt",  sln.qtree.colors(Qtree::Type::Mix ));
    to_file("data/qtree_busy.txt", sln.qtree.colors(Qtree::Type::Busy));
    
    std::string cmd = std::format("gnuplot -e \"filename='{}/{}_{}.png'\" draw/draw.gp > data/log.txt", foldername, i++, name);
    system(cmd.c_str());
}
