#include <cstddef>
#include <iostream>
#include "quantc/kernels.hpp"

using GateFn = void (*)(quantc::cplx *, int, int);

static void smoke_test(const char *name, GateFn gate)
{
    constexpr int n_qubits = 3;
    constexpr std::size_t size = 1ULL << n_qubits;

    alignas(32) quantc::cplx state[size] = {};
    state[0] = quantc::cplx(1.0, 0.0);

    gate(state, n_qubits, 2);

    std::cout << name << ": ";
    for (std::size_t i = 0; i < size; i++)
    {
        std::cout << " " << state[i].real();
    }
    std::cout << "\n";
}

int main()
{
    smoke_test("naive", quantc::naive::apply_H);
    smoke_test("omp", quantc::omp::apply_H);
    smoke_test("avx", quantc::avx::apply_H);

    return 0;
}