#pragma once

#include <chrono>
#include <filesystem>
#include <vector>

#include "types/Metric.hpp"

#include "gnuplot/exec.hpp"
#include "utils/to_file.hpp"


namespace demo {

void plot_collision(std::vector<std::pair<double,Metric>> data, std::string name = "") {
    std::vector<std::string> head{"-"};
    if (data.size() > 0) {
        const auto& [denst, metric] = *data.begin();
        for (const auto& [title, count] : metric.counter) {
            if (title == "edge_collision_check_unique") {
                head.push_back("Рёбра");
            } else if (title == "point_collision_check_unique") {
                head.push_back("Вершины");
            }
        }
    }

    name = name + "_collision_count";

    std::vector<io::Row> counts;
    for (const auto& [denst, metric] : data) {
        std::vector<double> row;

        row.push_back(denst);
        for (const auto& [title, count] : metric.counter) {
            if (title == "edge_collision_check_unique" || title == "point_collision_check_unique") {
                row.push_back(static_cast<double>(count));
            }
        }

        counts.emplace_back(std::move(row));
    }

    std::filesystem::create_directories("result/tmp/" + name);
    io::to_file("result/tmp/" + name + "/data.txt", counts, io::Options{std::move(head)});

    gnuplot::exec({
        .script_name     = "collision_count.gp",
        .script_dir_path = "gnuplot",
        .filename        = name,
        .output_dir_path = "result",
        .data_dir_path   = "result/tmp/" + name
    });
}

}
