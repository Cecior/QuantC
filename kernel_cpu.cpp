//
// Created by cesare on 7/19/26.
//

#include "kernel_cpu.h"
#include <memory>
#include <immintrin.h>

void apply_X_AVX(double* data, size_t size, int target) {
    size_t stride = 1ULL << (target + 1);

    if (stride < 4) {
        #pragma omp parallel for schedule(static)
        for(size_t block = 0; block < size; block += 2 * stride) {
            for(size_t offset = 0; offset < stride; offset += 2) {
                size_t idx1 = block ^ offset;
                size_t idx2 = idx1 ^ stride;

                std::swap(data[idx1], data[idx2]);
                std::swap(data[idx1 + 1], data[idx2 + 1]);
            }
        }

        return;
    }

    #pragma omp parallel for schedule(static)
    for(size_t block = 0; block < size; block += 2 * stride) {
        for(size_t offset = 0; offset < stride; offset += 4) {
            size_t idx1 = block ^ offset;
            size_t idx2 = idx1 ^ stride;

            __m256d vec1 = _mm256_load_pd(&data[idx1]);
            __m256d vec2 = _mm256_load_pd(&data[idx2]);

            _mm256_stream_pd(&data[idx1], vec2);
            _mm256_stream_pd(&data[idx2], vec1);
        }
    }
}
void apply_Y_AVX(double* data, size_t size, int target) {
    size_t stride = 1ULL << (target + 1);

    if (stride < 4) {
        #pragma omp parallel for schedule(static)
        for(size_t block = 0; block < size; block += 2 * stride) {
            for(size_t offset = 0; offset < stride; offset += 2) {
                size_t idx1 = block | offset;
                size_t idx2 = idx1 | stride;

                data[idx1] = -data[idx2 + 1];
                data[idx1 + 1] = data[idx2];

                data[idx2] = data[idx1 + 1];
                data[idx2 + 1] = data[idx1];
            }
        }
        return;
    }

    __m256d sign_V1 = _mm256_setr_pd(-1.0, 1.0, -1.0, 1.0);
    __m256d sign_V2 = _mm256_setr_pd(1.0, -1.0, 1.0, -1.0);

    #pragma omp parallel for schedule(static)
    for (size_t block = 0; block < size; block += 2 * stride) {
        for(size_t offset = 0; offset < stride; offset += 4) {
            size_t idx1 = block | offset;
            size_t idx2 = idx1 | stride;

            __m256d vec1 = _mm256_load_pd(&data[idx1]);
            __m256d vec2 = _mm256_load_pd(&data[idx2]);

            __m256d perm1 = _mm256_permute_pd(vec1, 0x5);
            __m256d perm2 = _mm256_permute_pd(vec2, 0x5);

            __m256d out1 = _mm256_mul_pd(perm1, sign_V1);
            __m256d out2 = _mm256_mul_pd(perm2, sign_V2);

            _mm256_stream_pd(&data[idx1], out1);
            _mm256_stream_pd(&data[idx2], out2);
        }
    }
}
void apply_Z_AVX(double* data, size_t size, int target) {
    size_t stride = 1ULL << (target + 1);

    if (stride < 4) {
        #pragma omp parallel for schedule(static)
        for(size_t block = 0; block < size; block += 2 * stride) {
            size_t base = block | stride;
            for(size_t offset = 0; offset < stride; offset += 2) {
                size_t idx = base | offset;
                data[idx] = -data[idx];
                data[idx + 1] = -data[idx + 1];
            }
        }
    }

    __m256d neg_ones = _mm256_set1_pd(-1.0);

    #pragma omp parallel for schedule(static)
    for (size_t block = 0; block < size; block += 2 * stride) {
        size_t base = block | stride;
        for(size_t offset = 0; offset < stride; offset += 4) {
            size_t idx = base | offset;
            __m256d vec = _mm256_load_pd(&data[idx]);
            vec = _mm256_mul_pd(vec, neg_ones);
            _mm256_stream_pd(&data[idx], -vec);
        }
    }
}