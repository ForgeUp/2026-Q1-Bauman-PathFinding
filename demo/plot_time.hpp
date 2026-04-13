#pragma once

#include <chrono>
#include <filesystem>
#include <vector>

#include "types/Metric.hpp"

#include "gnuplot/exec.hpp"
#include "utils/to_file.hpp"


namespace demo {

void plot_time(std::vector<std::pair<double,Metric>> data, std::string name = "") {
    std::vector<std::string> head{"-"};
    if (data.size() > 0) {
        const auto& [denst, metric] = *data.begin();
        for (const auto& [title, stamp] : metric.journal) {
            head.push_back(title);
        }
    }

    name = name + "_time";

    std::vector<io::Row> times;
    for (const auto& [denst, metric] : data) {
        std::vector<double> row;

        row.push_back(denst);
        for (const auto& [title, stamp] : metric.journal) {
            row.push_back(static_cast<double>(
                std::chrono::duration_cast<std::chrono::milliseconds>(stamp.acc).count()
            ));
        }

        times.emplace_back(std::move(row));
    }

    std::filesystem::create_directories("result/tmp/" + name);
    io::to_file("result/tmp/" + name + "/data.txt", times, io::Options{std::move(head)});

    gnuplot::exec({
        .script_name     = "perf.gp",
        .script_dir_path = "gnuplot",
        .filename        = name,
        .output_dir_path = "result",
        .data_dir_path   = "result/tmp/" + name
    });
}

}
