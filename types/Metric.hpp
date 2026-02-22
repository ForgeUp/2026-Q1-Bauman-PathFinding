#pragma once

#include <map>
#include <string>
#include <chrono>


class Metric {
public:
    struct Stamp {
        std::chrono::high_resolution_clock::duration acc;
        std::chrono::high_resolution_clock::time_point in;
        bool in_recorded{false};
    };

private:
    std::map<std::string,Stamp> journal;

public:
    using const_iterator =
        std::map<std::string, Stamp>::const_iterator;

    const_iterator begin() const noexcept {
        return journal.begin();
    }

    const_iterator end() const noexcept {
        return journal.end();
    }

public:
    void time_in(const std::string& name) {
        auto& stamp = journal[name];

        stamp.in = std::chrono::high_resolution_clock::now();
        stamp.in_recorded = true;
    }

    void time_out(const std::string& name) {
        auto& stamp = journal[name];

        if (!stamp.in_recorded) return;

        auto out = std::chrono::high_resolution_clock::now();
        stamp.in_recorded = false;

        stamp.acc += out - stamp.in;
    }

};
