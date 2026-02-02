#pragma once

#include "Lazy.hpp"


bool Lazy::check_enchancement_limit() {
    if (!is_path_not_found) return false;
    if (attempts < stgs.enhance_attempts_limit) return false;

    return true;
}
