#pragma once

#include <queue>
#include <memory>
#include <mutex>
#include <string>
#include <filesystem>

#include "types/Task.hpp"
#include "types/Solution.hpp"

#include "draw/draw.hpp"


class VisualizerBase {
public:
    struct Scene {
        Task task;
        Solution sln;
        std::string name;
        std::string dir;
    };

public:
    void picture(const Scene& sc) {}
};


class VisualizerService : public VisualizerBase {
private:
    std::queue<Scene> scenes;
    std::mutex scenes_mutex;

    std::condition_variable condition;
    bool stop{false};

    std::thread worker;

    std::string base_dir;
    std::mutex regdir_mutex;

public:
    VisualizerService(const std::string& base_dir_) : VisualizerBase(), base_dir(base_dir_) {
        if (!(std::filesystem::exists(base_dir) && std::filesystem::is_directory(base_dir))) {
            std::filesystem::create_directories(base_dir);
        }

        worker = std::thread([this]() {
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

                draw(sc.task, sc.sln, base_dir + '/' + sc.dir, sc.name);
            }
        });
    }

    ~VisualizerService() {
        {
            std::lock_guard<std::mutex> lock(scenes_mutex);
            stop = true;
        }
        condition.notify_all();
        worker.join();
    }

    void picture(const Scene& sc) {
        {
            std::lock_guard<std::mutex> lock(scenes_mutex);
            scenes.emplace(sc);
        }
        condition.notify_one();
    }

    std::string regdir(const std::string& sub_dir) {
        if (sub_dir == "") return sub_dir;
        {
            std::lock_guard<std::mutex> lock(regdir_mutex);

            std::string valid_sub_dir = sub_dir;
            std::string dirname = base_dir + '/' + valid_sub_dir;
            int32_t i{0};
            while (std::filesystem::exists(dirname) && std::filesystem::is_directory(dirname)) {
                valid_sub_dir = sub_dir + "_" + std::to_string(i++);
                dirname = base_dir + '/' + valid_sub_dir;
            }
            std::filesystem::create_directories(dirname);
            std::filesystem::create_directories(dirname + '/' + "data");
            return valid_sub_dir;
        }
    }

    VisualizerService(const VisualizerService&) = delete;
    VisualizerService& operator=(const VisualizerService&) = delete;

};
