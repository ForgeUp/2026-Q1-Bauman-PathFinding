#pragma once

#include <queue>
#include <memory>
#include <mutex>

#include "types/Task.hpp"
#include "types/Solution.hpp"

#include "draw/draw.hpp"


class VisualizerBase {
public:
    struct Scene {
        Task task;
        Solution sln;
        std::string name;
    };

public:
    void picture(const Scene& sc) {}
};


class Visualizer : public VisualizerBase {
private:
    std::queue<Scene> scenes;
    std::mutex scenes_mutex;

    std::condition_variable condition;
    bool stop{false};

    std::jthread worker;

public:
    Visualizer() : VisualizerBase() {
        worker = std::jthread([this]() {
            while (true) {
                Scene sc;

                {
                    std::unique_lock<std::mutex> lock(scenes_mutex);
                    condition.wait(lock, [this]() {
                        return stop || !scenes.empty();
                    });

                    if (stop && scenes.empty()) return;

                    sc = std::move(scenes.front());
                    scenes.pop();
                }

                draw(sc.task, sc.sln, sc.name);
            }
        });
    }

    ~Visualizer() {
        {
            std::lock_guard<std::mutex> lock(scenes_mutex);
            stop = true;
        }
        condition.notify_all();
    }

    void picture(const Scene& sc) {
        {
            std::lock_guard<std::mutex> lock(scenes_mutex);
            scenes.emplace(sc.task, sc.sln, sc.name);
        }
        condition.notify_one();
    }

    Visualizer(const Visualizer&) = delete;
    Visualizer& operator=(const Visualizer&) = delete;

};
