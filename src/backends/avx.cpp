#include <algorithm>
#include <cstdint>
#include <utility>
#include <immintrin.h>
#include <quantc/common.hpp>
#include <quantc/kernels.hpp>

namespace quantc::avx
{
    constexpr double sq12 = quantc::SQRT1_2;

    void apply_X(cplx *psi, int num_qubits, int target)
    {
        size_t size = 1ULL << num_qubits;
        size_t stride = 1ULL << target;

        if (stride < 2)
        {
#pragma omp parallel for schedule(static)
            for (int64_t block = 0; block < static_cast<int64_t>(size); block += 2 * stride)
            {
                for (size_t offset = 0; offset < stride; offset++)
                {
                    size_t idx1 = block ^ offset;
                    size_t idx2 = idx1 ^ stride;

                    std::swap(psi[idx1], psi[idx2]);
                }
            }

            return;
        }

        double *psi_data = reinterpret_cast<double *>(psi);

#pragma omp parallel for schedule(static)
        for (int64_t block = 0; block < static_cast<int64_t>(size); block += 2 * stride)
        {
            for (size_t offset = 0; offset < stride; offset += 2)
            {
                size_t idx1 = block ^ offset;
                size_t idx2 = idx1 ^ stride;

                __m256d vec1 = _mm256_load_pd(&psi_data[2 * idx1]);
                __m256d vec2 = _mm256_load_pd(&psi_data[2 * idx2]);

                _mm256_stream_pd(&psi_data[2 * idx1], vec2);
                _mm256_stream_pd(&psi_data[2 * idx2], vec1);
            }
        }
    }
    void apply_Y(cplx *psi, int num_qubits, int target)
    {
        size_t size = 1ULL << num_qubits;
        size_t stride = 1ULL << target;

        if (stride < 4)
        {
#pragma omp parallel for schedule(static)
            for (int64_t block = 0; block < static_cast<int64_t>(size); block += 2 * stride)
            {
                for (size_t offset = 0; offset < stride; offset++)
                {
                    size_t idx1 = block | offset;
                    size_t idx2 = idx1 | stride;

                    double r1 = psi[idx1].real();
                    double i1 = psi[idx1].imag();
                    double r2 = psi[idx2].real();
                    double i2 = psi[idx2].imag();

                    psi[idx1] = cplx(i2, -r2);
                    psi[idx2] = cplx(-i1, r1);
                }
            }
            return;
        }

        double *psi_data = reinterpret_cast<double *>(psi);

        __m256d sign_V1 = _mm256_setr_pd(-1.0, 1.0, -1.0, 1.0);
        __m256d sign_V2 = _mm256_setr_pd(1.0, -1.0, 1.0, -1.0);

#pragma omp parallel for schedule(static)
        for (int64_t block = 0; block < static_cast<int64_t>(size); block += 2 * stride)
        {
            for (size_t offset = 0; offset < stride; offset += 2)
            {
                size_t idx1 = block | offset;
                size_t idx2 = idx1 | stride;

                __m256d vec1 = _mm256_load_pd(&psi_data[2 * idx1]);
                __m256d vec2 = _mm256_load_pd(&psi_data[2 * idx2]);

                __m256d perm1 = _mm256_permute_pd(vec1, 0x5);
                __m256d perm2 = _mm256_permute_pd(vec2, 0x5);

                __m256d out1 = _mm256_mul_pd(perm2, sign_V2);
                __m256d out2 = _mm256_mul_pd(perm1, sign_V1);

                _mm256_stream_pd(&psi_data[2 * idx1], out1);
                _mm256_stream_pd(&psi_data[2 * idx2], out2);
            }
        }
    }
    void apply_Z(cplx *psi, int num_qubits, int target)
    {
        size_t size = 1ULL << num_qubits;
        size_t stride = 1ULL << target;

        if (stride < 4)
        {
#pragma omp parallel for schedule(static)
            for (int64_t block = 0; block < static_cast<int64_t>(size); block += 2 * stride)
            {
                size_t base = block | stride;
                for (size_t offset = 0; offset < stride; offset++)
                {
                    size_t idx = base | offset;
                    double r = psi[idx].real();
                    double i = psi[idx].imag();
                    psi[idx] = cplx(-r, -i);
                }
            }
            return;
        }

        double *psi_data = reinterpret_cast<double *>(psi);

        __m256d neg_ones = _mm256_set1_pd(-1.0);

#pragma omp parallel for schedule(static)
        for (int64_t block = 0; block < static_cast<int64_t>(size); block += 2 * stride)
        {
            size_t base = block | stride;
            for (size_t offset = 0; offset < stride; offset += 2)
            {
                size_t idx = base | offset;
                __m256d vec = _mm256_load_pd(&psi_data[idx]);
                vec = _mm256_mul_pd(vec, neg_ones);
                _mm256_stream_pd(&psi_data[idx], vec);
            }
        }
    }

    void apply_H(cplx *psi, int num_qubits, int target)
    {
        size_t size = 1ULL << num_qubits;
        size_t stride = 1ULL << target;

        if (stride < 4)
        {
#pragma omp parallel for schedule(static)
            for (int64_t block = 0; block < static_cast<int64_t>(size); block += 2 * stride)
            {
                for (size_t offset = 0; offset < stride; offset++)
                {
                    size_t idx1 = block | offset;
                    size_t idx2 = idx1 | stride;

                    double r1 = psi[idx1].real();
                    double i1 = psi[idx1].imag();
                    double r2 = psi[idx2].real();
                    double i2 = psi[idx2].imag();

                    psi[idx1] = cplx((r1 + r2) * sq12, (i1 + i2) * sq12);
                    psi[idx2] = cplx((r1 - r2) * sq12, (i1 - i2) * sq12);
                }
            }
            return;
        }

        double *psi_data = reinterpret_cast<double *>(psi);

        __m256d coef_v = _mm256_set1_pd(sq12);
#pragma omp parallel for schedule(static)
        for (int64_t block = 0; block < static_cast<int64_t>(size); block += 2 * stride)
        {
            for (size_t offset = 0; offset < stride; offset += 2)
            {
                size_t idx1 = block | offset;
                size_t idx2 = idx1 | stride;

                __m256d vec1 = _mm256_load_pd(&psi_data[2 * idx1]);
                __m256d vec2 = _mm256_load_pd(&psi_data[2 * idx2]);

                __m256d contrib1 = _mm256_mul_pd(vec1, coef_v);
                __m256d contrib2 = _mm256_mul_pd(vec2, coef_v);

                vec1 = _mm256_add_pd(contrib1, contrib2);
                vec2 = _mm256_sub_pd(contrib1, contrib2);

                _mm256_stream_pd(&psi_data[2 * idx1], vec1);
                _mm256_stream_pd(&psi_data[2 * idx2], vec2);
            }
        }
    }

    void apply_CX(cplx *psi, int num_qubits, int control, int target)
    {
        size_t size = 1ULL << num_qubits;
        size_t stride_tg = 1ULL << target;
        size_t stride_ctl = 1ULL << control;

        size_t max_v = std::max(stride_tg, stride_ctl);
        size_t min_v = std::min(stride_tg, stride_ctl);

        if (min_v < 4)
        {
#pragma omp parallel for schedule(static)
            for (int64_t high = 0; high < static_cast<int64_t>(size); high += 2 * max_v)
            {
                for (size_t mid = 0; mid < max_v; mid += 2 * min_v)
                {
                    for (size_t low = 0; low < min_v; low++)
                    {
                        size_t idx1 = high | mid | low | stride_ctl;
                        size_t idx2 = idx1 | stride_tg;

                        std::swap(psi[idx1], psi[idx2]);
                    }
                }
            }
            return;
        }

        double *psi_data = reinterpret_cast<double *>(psi);

#pragma omp parallel for schedule(static)
        for (int64_t high = 0; high < static_cast<int64_t>(size); high += 2 * max_v)
        {
            for (size_t mid = 0; mid < max_v; mid += 2 * min_v)
            {
                for (size_t low = 0; low < min_v; low += 2)
                {
                    size_t idx1 = high | mid | low | stride_ctl;
                    size_t idx2 = idx1 | stride_tg;

                    __m256d vec1 = _mm256_load_pd(&psi_data[2 * idx1]);
                    __m256d vec2 = _mm256_load_pd(&psi_data[2 * idx2]);

                    _mm256_stream_pd(&psi_data[2 * idx1], vec2);
                    _mm256_stream_pd(&psi_data[2 * idx2], vec1);
                }
            }
        }
    }
}