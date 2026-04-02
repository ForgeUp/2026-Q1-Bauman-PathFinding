
#include "draw/VisualizerService.hpp"
#include "types/SolverSettings.hpp"
#include "demo/demo.hpp"
#include "solver/Solver/all.hpp"

#include "utils/logger.hpp"


int main() {
    Logger::set_stream_to_file("result/log_solve.txt");
    DrawLogger::set_stream_to_file("result/log_draw.txt");
    VisualizerService srv_visual("result");
    
    using Solver = solver::ObstacleOffset;

    SolverSettings stgs = {
        .initial_nodes_count = 500,
        .bridge_standard_deviation = 2,
        .gauss_standard_deviation = 1,
        .enhance_attempts_limit = 10
    };

    demo::Data data {
        .stgs = stgs,
        .srv_visual = srv_visual,
        
        .runs_per_area = 3
    };

    // demo::test_standard_area<Solver>(data);
    // demo::test_rand_area<Solver>(data);
    demo::test_classic_and_lazy_PRM(srv_visual);

    return 0;
}
