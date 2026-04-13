#pragma once

#include <string>
#include <filesystem>

#include "types/Task.hpp"
#include "types/Solution.hpp"
#include "types/Graph.hpp"

#include "utils/Timer.hpp"
#include "utils/logger.hpp"
#include "utils/concat.hpp"
#include "utils/to_file.hpp"

#include "gnuplot/exec.hpp"


void draw(const Task& task, const Solution& sln, const std::string& dir, const std::string& pic) {
    DrawLogger::in();
    Timer timer;

    std::filesystem::path data_dir = dir + '/' + "data";

    io::to_file(data_dir / "area.gp", concat(
        "x_min="  , task.area.x_min, "\n",
        "x_max="  , task.area.x_max, "\n",
        "y_min="  , task.area.y_min, "\n",
        "y_max="  , task.area.y_max, "\n",
        "x_start=", task.start.x   , "\n",
        "y_start=", task.start.y   , "\n",
        "x_end="  , task.end.x     , "\n",
        "y_end="  , task.end.y     , "\n"
    ));

    io::to_file(data_dir / "polygons.txt",      task.area.rocks);
    io::to_file(data_dir / "grid.txt",          sln.grid);
    io::to_file(data_dir / "enhance_p.txt",     sln.enhance.verts);
    io::to_file(data_dir / "enhance_e.txt",     sln.enhance);
    io::to_file(data_dir / "examined.txt",      sln.examined);
    io::to_file(data_dir / "invalid.txt",       sln.invalid);
    io::to_file(data_dir / "invalid_all_p.txt", sln.invalid_all.verts);
    io::to_file(data_dir / "invalid_all_e.txt", sln.invalid_all);
    io::to_file(data_dir / "path.txt",          sln.path);
    io::to_file(data_dir / "qtree.txt",         sln.qtree);
    io::to_file(data_dir / "qtree_free.txt",    sln.qtree.colors(Qtree::Type::Free));
    io::to_file(data_dir / "qtree_mix.txt",     sln.qtree.colors(Qtree::Type::Mix ));
    io::to_file(data_dir / "qtree_busy.txt",    sln.qtree.colors(Qtree::Type::Busy));
    
    auto to_file_time = timer.tick();

    gnuplot::exec({
        .script_name     = "draw.gp",
        .script_dir_path = "draw",
        .filename        = pic,
        .output_dir_path = dir,
        .data_dir_path   = data_dir.string()
    });

    auto gnuplot_time = timer.tick() - to_file_time;

    DrawLogger::log("file io time consume: {}", to_file_time);
    DrawLogger::log("gnuplot time consume: {}", gnuplot_time);

    DrawLogger::out();
}
