#include <cstddef>
#include <memory>
#include <immintrin.h>
#include <utils.hpp>

const double sq12 = qsim::sqrt1_2;

void apply_H_raw(double *data, size_t size, int target)
{
    size_t stride = 1ULL << (target + 1);

    for (size_t block = 0; block < size; block += 2 * stride)
    {
        for (size_t offset = 0; offset < stride; offset += 2)
        {
            size_t idx1 = block | offset;
            size_t idx2 = idx1 | stride;

            double r1 = data[idx1];
            double i1 = data[idx1 + 1];
            double r2 = data[idx2];
            double i2 = data[idx2 + 1];

            data[idx1] = (r1 + r2) * sq12;
            data[idx1 + 1] = (i1 + i2) * sq12;
            data[idx2] = (r1 - r2) * sq12;
            data[idx2 + 1] = (i1 - i2) * sq12;
        }
    }
}