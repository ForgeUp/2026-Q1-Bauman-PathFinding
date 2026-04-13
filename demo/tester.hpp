#pragma once

#include <string>
#include <format>
#include <algorithm>
#include <vector>

#include "demo/Data.hpp"
#include "demo/stat_log.hpp"

#include "types/Metric.hpp"
#include "types/Services.hpp"

#include "draw/VisualizerService.hpp"

#include "utils/logger.hpp"


namespace demo {

template <typename Solver>
Metric tester(Data& data) {
    std::string output_folder = std::format("{:%Y-%m-%d %H-%M-%S} [{}]", 
        std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()), 
        data.folder_mark
    );
    Services srvs = {
        .visual = VisualizerAdapter(data.srv_visual, output_folder) 
    };
    
    auto solver = Solver(data.task, data.stgs, srvs);
    auto sln = solver.run();

    demo::stat_log(sln.metric, data.folder_mark);

    return sln.metric;
}

}
