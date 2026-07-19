//
// Created by cesare on 7/19/26.
//

#ifndef QUANTC_KERNEL_CPU_H
#define QUANTC_KERNEL_CPU_H

#pragma once
#include <cstddef>

void apply_X_AVX(double* data, size_t size, int target);
void apply_Y_AVX(double* data, size_t size, int target);
void apply_Z_AVX(double* data, size_t size, int target);

#endif //QUANTC_KERNEL_CPU_H
