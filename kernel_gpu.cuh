//
// Created by cesare on 7/21/26.
//

#ifndef QUANTC_KERNEL_GPU_H
#define QUANTC_KERNEL_GPU_H

#include <cuda_runtime.h>

void launch_apply_X(double* data, size_t num_states, int target, int blocks, int threads);
void launch_apply_Y(double* data, size_t num_states, int target, int blocks, int threads);
void launch_apply_Z(double* data, size_t num_states, int target, int blocks, int threads);
void launch_apply_H(double* data, size_t num_states, int target, int blocks, int threads);

#endif //QUANTC_KERNEL_GPU_H
