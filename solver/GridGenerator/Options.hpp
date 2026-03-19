#pragma once


namespace GridGenerator {

struct Options {
    bool check_collision{true};
    bool connect{false};
    int32_t nodes_count{0};
};

}
