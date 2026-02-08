#pragma once


namespace math {

template <typename T>
int sign(T x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

}
