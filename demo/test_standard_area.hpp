#pragma once

#include "demo/Data.hpp"
#include "demo/examples.hpp"
#include "demo/tester.hpp"


namespace demo {

template <typename Solver>
void test_standard_area(Data& data) {
    for (const auto& [title, task] : Examples{}.tasks) {
        data.task = task;
        data.folder_mark = title;
        
        demo::tester<Solver>(data);
    }
}
    
}
