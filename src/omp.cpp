#include <cstddef>
#include <memory>
#include <immintrin.h>
#include <utils.hpp>

const double sq12 = qsim::sqrt1_2;

void apply_X_raw(double *data, size_t size, int target)
{
    size_t stride = 1ULL << (target + 1);

#pragma omp parallel for schedule(static)
    for (int64_t block = 0; block < size; block += 2 * stride)
    {
        for (size_t offset = 0; offset < stride; offset += 2)
        {
            size_t idx1 = block | offset;
            size_t idx2 = idx1 | stride;

            std::swap(data[idx1], data[idx2]);
            std::swap(data[idx1 + 1], data[idx2 + 1]);
        }
    }
}

void apply_Y_raw(double *data, size_t size, int target)
{
    size_t stride = 1ULL << (target + 1);

#pragma omp parallel for schedule(static)
    for (int64_t block = 0; block < size; block += 2 * stride)
    {
        for (size_t offset = 0; offset < stride; offset += 2)
        {
            size_t idx1 = block | offset;
            size_t idx2 = idx1 | stride;

            double r1 = data[idx1];
            double i1 = data[idx1 + 1];
            double r2 = data[idx2];
            double i2 = data[idx2 + 1];

            data[idx1] = i2;
            data[idx1 + 1] = -r2;
            data[idx2] = -i1;
            data[idx2 + 1] = r1;
        }
    }
}

void apply_Z_raw(double *data, size_t size, int target)
{
    size_t stride = 1ULL << (target + 1);

#pragma omp parallel for schedule(static)
    for (int64_t block = 0; block < size; block += 2 * stride)
    {
        for (size_t offset = 0; offset < stride; offset += 2)
        {
            size_t idx = block | offset | stride;

            double r = data[idx];
            double i = data[idx + 1];

            data[idx] = -r;
            data[idx + 1] = -i;
        }
    }
}

void apply_H_omp(double *data, size_t size, int target)
{
    size_t stride = 1ULL << (target + 1);

#pragma omp parallel for schedule(static)
    for (int64_t block = 0; block < size; block += 2 * stride)
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

void apply_CX_raw(double *data, size_t size, int control, int target)
{
    size_t stride_tg = 1ULL << (target + 1);
    size_t stride_ctl = 1ULL << (control + 1);

    size_t max_v = std::max(stride_tg, stride_ctl);
    size_t min_v = std::min(stride_tg, stride_ctl);

#pragma omp parallel for schedule(static)
    for (int64_t high = 0; high < size; high += 2 * max_v)
    {
        for (size_t mid = 0; mid < max_v; mid += 2 * min_v)
        {
            for (size_t low = 0; low < min_v; low += 2)
            {
                size_t idx1 = high | mid | low | stride_ctl;
                size_t idx2 = idx1 | stride_tg;

                std::swap(data[idx1], data[idx2]);
                std::swap(data[idx1 + 1], data[idx2 + 1]);
            }
        }
    }
}