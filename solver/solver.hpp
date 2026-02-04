#pragma once

#include "BaseSolver.hpp"

#include "Lazy/Lazy.hpp"
#include "LazyNaiveGridSealing/LazyNaiveGridSealing.hpp"
#include "LazyInitialOffset/LazyInitialOffset.hpp"


namespace solver {
    using lazy = BaseSolver<Lazy>; 
    using lazy_naive = BaseSolver<LazyNaiveGridSealing>;
    using lazy_naive_with_offset = BaseSolver<LazyInitialOffset>;
}
