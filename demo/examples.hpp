#pragma once

#include <map>

#include "types/Task.hpp"
#include "types/Area.hpp"
#include "types/Rock.hpp"
#include "types/Point.hpp"


namespace demo {

struct Examples {
    std::map<std::string,Task> tasks = {
        {
            "simple_tunnel",
            Task{
                .start = Point(10, 10),
                .end   = Point(90, 90),
                .area  = Area{
                    .x_min = 0,
                    .x_max = 100,
                    .y_min = 0,
                    .y_max = 100,

                    .rocks = std::vector<Rock>{
                        Rock{{{20,0},  {20,45}, {80,45},{80,0} }},
                        Rock{{{20,100},{80,100},{80,55},{20,55}}},
                    }
                }
            }
        },
        {
            "narrow_snake",
            Task{
                .start = Point(10, 10),
                .end   = Point(90, 90),
                .area  = Area{
                    .x_min = 0,
                    .x_max = 100,
                    .y_min = 0,
                    .y_max = 100,

                    .rocks = std::vector<Rock>{
                        Rock{{ {15,0},{15,80},{25,80},{25,10},{55,10},{55,80},{65,80},{65,10},{85,10},{85,0} }},
                        Rock{{ {15,100},{85,100},{85,20},{75,20},{75,90},{45,90},{45,20},{35,20},{35,90},{15,90} }},
                    }
                }
            }
        },
        {
            "triangle",
            Task{
                .start = Point(10, 10),
                .end   = Point(90, 90),
                .area  = Area{
                    .x_min = 0,
                    .x_max = 100,
                    .y_min = 0,
                    .y_max = 100,

                    .rocks = std::vector<Rock>{
                        Rock{{ {20, 20}, {80, 80}, {80, 20} }}
                    }
                }
            }
        }
    };
};
    
}
