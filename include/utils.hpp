#pragma once

#include <iostream>
#include <chrono>
#include <fstream>

namespace qsim
{
    constexpr double sqrt1_2 = 0.70710678118654752440;

    inline double *aligned_state_vector(int nq)
    {
        size_t n_elem = 1ULL << (nq + 1);
        size_t size_in_bytes = n_elem * sizeof(double);

        double *data = static_cast<double *>(aligned_alloc(32, size_in_bytes));

        for (int i = 0; i < n_elem; i++)
        {
            data[i] = 0.0;
        }
        data[0] = 1.0;

        return data;
    }

    inline size_t insert_hole(size_t idx, int target)
    {
        size_t high = (idx >> target) << (target + 1);
        size_t low = idx & ((1ULL << target) - 1);

        return high | low;
    }
}