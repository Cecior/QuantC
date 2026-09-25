#pragma once
#include <cstddef>
#include <complex>

namespace quantc
{
    using cplx = std::complex<double>;
}

namespace quantc::naive
{
    void apply_X(cplx *psi, int num_qubits, int target);
    void apply_Y(cplx *psi, int num_qubits, int target);
    void apply_Z(cplx *psi, int num_qubits, int target);
    void apply_H(cplx *psi, int num_qubits, int target);
    void apply_CX(cplx *psi, int num_qubits, int control, int target);
}

namespace quantc::omp
{
    void apply_X(cplx *psi, int num_qubits, int target);
    void apply_Y(cplx *psi, int num_qubits, int target);
    void apply_Z(cplx *psi, int num_qubits, int target);
    void apply_H(cplx *psi, int num_qubits, int target);
    void apply_CX(cplx *psi, int num_qubits, int control, int target);
}

namespace quantc::avx
{
    void apply_X(cplx *psi, int num_qubits, int target);
    void apply_Y(cplx *psi, int num_qubits, int target);
    void apply_Z(cplx *psi, int num_qubits, int target);
    void apply_H(cplx *psi, int num_qubits, int target);
    void apply_CX(cplx *psi, int num_qubits, int control, int target);
}