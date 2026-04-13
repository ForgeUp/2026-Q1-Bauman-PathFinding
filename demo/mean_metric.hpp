#pragma once

#include <vector>

#include "types/Metric.hpp"


namespace demo {

Metric mean_metric(const std::vector<Metric>& metrics) {
    if (metrics.size() < 1) return Metric{};

    Metric mean;
    for (const auto& metric : metrics) {
        for (const auto& [name, stamp] : metric.journal) {
            mean.journal[name].acc += stamp.acc;
        }
        for (const auto& [name, count] : metric.counter) {
            mean.counter[name] += count;
        }
    }
    for (const auto& [name, stamp] : mean.journal) {
        mean.journal[name].acc /= metrics.size();
    }
    for (const auto& [name, count] : mean.counter) {
        mean.counter[name] /= metrics.size();
    }
    
    return mean;
}

}
