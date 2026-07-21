//
// Created by cesare on 7/19/26.
//

#ifndef QUANTC_KERNEL_CPU_H
#define QUANTC_KERNEL_CPU_H

#pragma once
#include <complex>
#include <cstddef>

using Complex = std::complex<double>;

void apply_X_AVX(double* data, size_t size, int target);




void apply_Y_AVX(double* data, size_t size, int target);




void apply_Z_AVX(double* data, size_t size, int target);

void apply_H_raw(double* data, size_t size, int target);
void apply_H_omp(double* data, size_t size, int target);
void apply_H_AVX(double* data, size_t size, int target);

#endif //QUANTC_KERNEL_CPU_H
