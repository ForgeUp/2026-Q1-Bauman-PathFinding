#pragma once

#include <string>
#include <format>
#include <algorithm>
#include <vector>

#include "demo/Data.hpp"

#include "types/Services.hpp"

#include "draw/VisualizerService.hpp"

#include "utils/Timer.hpp"
#include "utils/logger.hpp"


namespace demo {

template <typename Solver>
void tester(Data& data) {
    Logger::buff_on();
    Logger::log("Run {}", data.folder_mark);
    Logger::in();
    
    std::string output_folder = std::format("{:%Y-%m-%d %H-%M-%S} [{}]", 
        std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()), 
        data.folder_mark
    );
    Services srvs = {
        .visual = VisualizerAdapter(data.srv_visual, output_folder) 
    };
    Timer timer;
    
    auto solver = Solver(data.task, data.stgs, srvs);
    auto sln = solver.run();
    
    auto total = timer.tick();

    std::vector<std::pair<std::string,Metric::Stamp>> metric;
    for (const auto& [name, stamp] : sln.metric) {
        metric.emplace_back(name, stamp);
    }
    std::sort(metric.begin(), metric.end(), [](auto& l, auto& r) {
        return l.second.acc > r.second.acc;
    });

    Logger::log("Total time consume: {}", total);
    Logger::in();
    for (const auto& [name, stamp] : metric) {
        Logger::log("{:<30} | {:6} | {:10} | {:6.2f}%", 
            name,
            stamp.counter,
            std::chrono::duration_cast<std::chrono::milliseconds>(stamp.acc), 
            100.0 * stamp.acc / total
        );
    }
    Logger::out();

    Logger::log("Collision checks:");
    Logger::in();
    for (const auto& [name, count] : sln.metric.counter) {
        Logger::log("{:<30} | {:8}", name, count);
    }
    Logger::out();

    Logger::out();
    Logger::buff_off();
}

}
