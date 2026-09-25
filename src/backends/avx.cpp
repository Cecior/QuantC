#include <cstddef>
#include <memory>
#include <immintrin.h>
#include <utils.hpp>

const double sq12 = qsim::sqrt1_2;

void apply_X_avx(double *data, size_t size, int target)
{
    size_t stride = 1ULL << (target + 1);

    if (stride < 4)
    {
#pragma omp parallel for schedule(static)
        for (int64_t block = 0; block < size; block += 2 * stride)
        {
            for (size_t offset = 0; offset < stride; offset += 2)
            {
                size_t idx1 = block ^ offset;
                size_t idx2 = idx1 ^ stride;

                std::swap(data[idx1], data[idx2]);
                std::swap(data[idx1 + 1], data[idx2 + 1]);
            }
        }

        return;
    }

#pragma omp parallel for schedule(static)
    for (int64_t block = 0; block < size; block += 2 * stride)
    {
        for (size_t offset = 0; offset < stride; offset += 4)
        {
            size_t idx1 = block ^ offset;
            size_t idx2 = idx1 ^ stride;

            __m256d vec1 = _mm256_load_pd(&data[idx1]);
            __m256d vec2 = _mm256_load_pd(&data[idx2]);

            _mm256_stream_pd(&data[idx1], vec2);
            _mm256_stream_pd(&data[idx2], vec1);
        }
    }
}
void apply_Y_avx(double *data, size_t size, int target)
{
    size_t stride = 1ULL << (target + 1);

    if (stride < 4)
    {
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
        return;
    }

    __m256d sign_V1 = _mm256_setr_pd(-1.0, 1.0, -1.0, 1.0);
    __m256d sign_V2 = _mm256_setr_pd(1.0, -1.0, 1.0, -1.0);

#pragma omp parallel for schedule(static)
    for (int64_t block = 0; block < size; block += 2 * stride)
    {
        for (size_t offset = 0; offset < stride; offset += 4)
        {
            size_t idx1 = block | offset;
            size_t idx2 = idx1 | stride;

            __m256d vec1 = _mm256_load_pd(&data[idx1]);
            __m256d vec2 = _mm256_load_pd(&data[idx2]);

            __m256d perm1 = _mm256_permute_pd(vec1, 0x5);
            __m256d perm2 = _mm256_permute_pd(vec2, 0x5);

            __m256d out1 = _mm256_mul_pd(perm2, sign_V2);
            __m256d out2 = _mm256_mul_pd(perm1, sign_V1);

            _mm256_stream_pd(&data[idx1], out1);
            _mm256_stream_pd(&data[idx2], out2);
        }
    }
}
void apply_Z_avx(double *data, size_t size, int target)
{
    size_t stride = 1ULL << (target + 1);

    if (stride < 4)
    {
#pragma omp parallel for schedule(static)
        for (int64_t block = 0; block < size; block += 2 * stride)
        {
            size_t base = block | stride;
            for (size_t offset = 0; offset < stride; offset += 2)
            {
                size_t idx = base | offset;
                data[idx] = -data[idx];
                data[idx + 1] = -data[idx + 1];
            }
        }
        return;
    }

    __m256d neg_ones = _mm256_set1_pd(-1.0);

#pragma omp parallel for schedule(static)
    for (int64_t block = 0; block < size; block += 2 * stride)
    {
        size_t base = block | stride;
        for (size_t offset = 0; offset < stride; offset += 4)
        {
            size_t idx = base | offset;
            __m256d vec = _mm256_load_pd(&data[idx]);
            vec = _mm256_mul_pd(vec, neg_ones);
            _mm256_stream_pd(&data[idx], vec);
        }
    }
}

void apply_H_avx(double *data, size_t size, int target)
{
    size_t stride = 1ULL << (target + 1);

    if (stride < 4)
    {
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
        return;
    }

    __m256d coef_v = _mm256_set1_pd(sq12);
#pragma omp parallel for schedule(static)
    for (int64_t block = 0; block < size; block += 2 * stride)
    {
        for (size_t offset = 0; offset < stride; offset += 4)
        {
            size_t idx1 = block | offset;
            size_t idx2 = idx1 | stride;

            __m256d vec1 = _mm256_load_pd(&data[idx1]);
            __m256d vec2 = _mm256_load_pd(&data[idx2]);

            __m256d contrib1 = _mm256_mul_pd(vec1, coef_v);
            __m256d contrib2 = _mm256_mul_pd(vec2, coef_v);

            vec1 = _mm256_add_pd(contrib1, contrib2);
            vec2 = _mm256_sub_pd(contrib1, contrib2);

            _mm256_stream_pd(&data[idx1], vec1);
            _mm256_stream_pd(&data[idx2], vec2);
        }
    }
}

void apply_CX_avx(double *data, size_t size, int control, int target)
{
    size_t stride_tg = 1ULL << (target + 1);
    size_t stride_ctl = 1ULL << (control + 1);

    size_t max_v = std::max(stride_tg, stride_ctl);
    size_t min_v = std::min(stride_tg, stride_ctl);

    if (min_v < 4)
    {
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
        return;
    }

    for (size_t high = 0; high < size; high += 2 * max_v)
    {
        for (size_t mid = 0; mid < max_v; mid += 2 * min_v)
        {
            for (size_t low = 0; low < min_v; low += 4)
            {
                size_t idx1 = high | mid | low | stride_ctl;
                size_t idx2 = idx1 | stride_tg;

                __m256d vec1 = _mm256_load_pd(&data[idx1]);
                __m256d vec2 = _mm256_load_pd(&data[idx2]);

                _mm256_stream_pd(&data[idx1], vec2);
                _mm256_stream_pd(&data[idx2], vec1);
            }
        }
    }
}