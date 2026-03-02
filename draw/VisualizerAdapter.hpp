#pragma once

#include <string>
#include "format"

#include "draw/VisualizerService.hpp"


class VisualizerAdapter : public VisualizerBase {
public:
    struct Scene {
        Task task;
        Solution sln;
        std::string name;
    };

private:
    VisualizerService& srv;

    std::string dirname{""};
    int32_t counter{1};

public:
    VisualizerAdapter(VisualizerService& srv_, const std::string& dirname_) : 
        srv(srv_),
        dirname(srv.regdir(dirname_))
    {}

    void picture(const Scene& sc) {
        srv.picture({
            .task = sc.task,
            .sln  = sc.sln,
            .name = std::format("{}_{}", counter++, sc.name),
            .dir  = dirname
        });
    }
};
