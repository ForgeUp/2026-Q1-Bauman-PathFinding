#pragma once

#include <iostream>
#include <string>
#include <format>
#include <filesystem>

#include <utils/concat.hpp>


namespace gnuplot {

struct Options {
    std::string script_name;
    std::string script_dir_path;
    std::string filename;
    std::string output_dir_path;
    std::string data_dir_path;
};

void exec(const Options& opts) {
    std::string script_path = opts.script_dir_path + '/' + opts.script_name;
    if (!std::filesystem::exists(opts.script_dir_path) ||  !std::filesystem::is_directory(opts.script_dir_path)) {
        std::cerr << "Error: Can't locate directory with gnuplot script: " << opts.script_dir_path << '\n';
        return;
    }
    if (!std::filesystem::exists(script_path)) {
        std::cerr << "Error: Can't locate gnuplot script: " << script_path << '\n';
        return;
    }
    if (!std::filesystem::exists(opts.output_dir_path) ||  !std::filesystem::is_directory(opts.output_dir_path)) {
        std::cerr << "Warning: Can't locate output directory for gnuplot: " << opts.output_dir_path << " - directory is created" << '\n';
        std::filesystem::create_directories(opts.output_dir_path);
        return;
    }

    std::string cmd = concat(
        "gnuplot -e ",
        "\"",
        std::format("filename='{}.png'", opts.filename), "; ",
        std::format("output_dir='{}'", opts.output_dir_path),   "; ",
        std::format("data_dir='{}'", opts.data_dir_path), 
        "\" ",
        script_path, 
        " > ", "\"", opts.data_dir_path + '/' + "log.txt", "\" ",
        "2>&1"
    );
    system(cmd.c_str());
}

}
