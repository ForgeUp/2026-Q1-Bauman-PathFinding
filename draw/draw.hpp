#pragma once

#include <format>
#include <string>
#include <filesystem>

#include "types/Task.hpp"
#include "types/Solution.hpp"

#include "to_file.hpp"


void draw(const Task& task, const Solution& sln, const std::string& dir, const std::string& pic) {
    std::filesystem::path data_dir = dir + '/' + "data";

    auto to_string = [&]<typename ...Args>(Args&&... args) -> std::string {
        std::ostringstream oss;
        (oss << ... << args);
        return oss.str();
    };

    to_file(data_dir / "area.gp", to_string(
        "x_min="  , task.area.x_min, "\n",
        "x_max="  , task.area.x_max, "\n",
        "y_min="  , task.area.y_min, "\n",
        "y_max="  , task.area.y_max, "\n",
        "x_start=", task.start.x   , "\n",
        "y_start=", task.start.y   , "\n",
        "x_end="  , task.end.x     , "\n",
        "y_end="  , task.end.y     , "\n"
    ));

    to_file(data_dir / "polygons.txt",    task.area.rocks);
    to_file(data_dir / "grid.txt",        sln.grid);
    to_file(data_dir / "enhance.txt",     sln.enhance);
    to_file(data_dir / "examined.txt",    sln.examined);
    to_file(data_dir / "invalid.txt",     sln.invalid);
    to_file(data_dir / "invalid_all.txt", sln.invalid_all);
    to_file(data_dir / "path.txt",        sln.path);
    to_file(data_dir / "qtree.txt",       sln.qtree);
    to_file(data_dir / "qtree_free.txt",  sln.qtree.colors(Qtree::Type::Free));
    to_file(data_dir / "qtree_mix.txt",   sln.qtree.colors(Qtree::Type::Mix ));
    to_file(data_dir / "qtree_busy.txt",  sln.qtree.colors(Qtree::Type::Busy));
    
    std::string cmd = to_string(
        "gnuplot -e ",
        "\"",
        std::format("filename='{}.png'", pic), "; ",
        std::format("output_dir='{}'", dir),   "; ",
        std::format("data_dir='{}'", data_dir.string()), 
        "\" ",
        "draw/draw.gp > ", "\"", data_dir.string() + '/' + "log.txt", "\""
    );
    system(cmd.c_str());
}
