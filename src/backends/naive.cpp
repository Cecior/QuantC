#include <algorithm>
#include <utility>
#include "quantc/common.hpp"
#include "quantc/kernels.hpp"

namespace quantc::naive
{
    constexpr double sq12 = SQRT1_2;

    void apply_X(cplx *psi, int num_qubits, int target)
    {
        size_t size = 1ULL << num_qubits;
        size_t stride = 1ULL << target;

        for (size_t block = 0; block < size; block += 2 * stride)
        {
            for (size_t offset = 0; offset < stride; offset++)
            {
                size_t idx1 = block | offset;
                size_t idx2 = idx1 | stride;

                std::swap(psi[idx1], psi[idx2]);
            }
        }
    }

    void apply_Y(cplx *psi, int num_qubits, int target)
    {
        size_t size = 1ULL << num_qubits;
        size_t stride = 1ULL << target;

        for (size_t block = 0; block < size; block += 2 * stride)
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
    }

    void apply_Z(cplx *psi, int num_qubits, int target)
    {
        size_t size = 1ULL << num_qubits;
        size_t stride = 1ULL << target;

        for (size_t block = 0; block < size; block += 2 * stride)
        {
            for (size_t offset = 0; offset < stride; offset++)
            {
                size_t idx = block | offset | stride;

                double r = psi[idx].real();
                double i = psi[idx].imag();

                psi[idx] = cplx(-r, -i);
            }
        }
    }

    void apply_H(cplx *psi, int num_qubits, int target)
    {
        size_t size = 1ULL << num_qubits;
        size_t stride = 1ULL << target;

        for (size_t block = 0; block < size; block += 2 * stride)
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
    }

    void apply_CX(cplx *psi, int num_qubits, int control, int target)
    {
        size_t size = 1ULL << num_qubits;
        size_t stride_tg = 1ULL << target;
        size_t stride_ctl = 1ULL << control;

        size_t max_v = std::max(stride_tg, stride_ctl);
        size_t min_v = std::min(stride_tg, stride_ctl);

        for (size_t high = 0; high < size; high += 2 * max_v)
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
    }
}