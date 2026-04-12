
#include <chrono>
#include <vector>

#include "types/Solution.hpp"
#include "types/Metric.hpp"

#include "utils/logger.hpp"


namespace demo {

void stat_log(const Metric& metric, const std::string& solver_name) {
    Logger::buff_on();
    Logger::log("Result of {}", solver_name);
    Logger::in();

    std::vector<std::pair<std::string,Metric::Stamp>> data;
    for (const auto& [name, stamp] : metric) {
        data.emplace_back(name, stamp);
    }
    std::sort(data.begin(), data.end(), [](auto& l, auto& r) {
        return l.second.acc > r.second.acc;
    });

    Logger::log("Total time consume: {}", std::chrono::duration_cast<std::chrono::milliseconds>(metric.total));
    Logger::in();
    for (const auto& [name, stamp] : data) {
        Logger::log("{:<30} | {:6} | {:10} | {:6.2f}%", 
            name,
            stamp.counter,
            std::chrono::duration_cast<std::chrono::milliseconds>(stamp.acc), 
            100.0 * stamp.acc / metric.total
        );
    }
    Logger::out();

    Logger::log("Collision checks:");
    Logger::in();
    for (const auto& [name, count] : metric.counter) {
        Logger::log("{:<30} | {:8}", name, count);
    }
    Logger::out();

    Logger::out();
    Logger::buff_off();
}

}
