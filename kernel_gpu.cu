//
// Created by cesare on 7/21/26.
//

#include "kernel_gpu.cuh"
#include <device_launch_parameters.h>
#include <cmath>

__global__ void apply_X_kernel(double* data, size_t num_states, int target) {
    size_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= num_states / 2) return;

    double2* c_data = reinterpret_cast<double2*>(data);

    size_t stride = 1ULL << target;
    size_t block = tid / stride;
    size_t offset = tid % stride;

    size_t idx1 = (block * 2 * stride) + offset;
    size_t idx2 = idx1 + stride;

    double2 temp = c_data[idx1];
    c_data[idx1] = c_data[idx2];
    c_data[idx2] = temp;
}
void launch_apply_X(double *data, size_t num_states, int target, int blocks, int threads) {
    apply_X_kernel<<<blocks, threads>>>(data, num_states, target);
}

__global__ void apply_Y_kernel(double* data, size_t num_states, int target) {
    size_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= num_states / 2) return;

    double2* c_data = reinterpret_cast<double2*>(data);

    size_t stride = 1ULL << target;
    size_t block = tid / stride;
    size_t offset = tid % stride;

    size_t idx1 = (block * 2 * stride) + offset;
    size_t idx2 = idx1 + stride;

    double r1 = c_data[idx1].x;
    double i1 = c_data[idx1].y;

    double r2 = c_data[idx2].x;
    double i2 = c_data[idx2].y;

    c_data[idx1].x = -i2;
    c_data[idx1].y = r2;

    c_data[idx2].x = i1;
    c_data[idx2].y = -r1;
}
void launch_apply_Y(double *data, size_t num_states, int target, int blocks, int threads) {
    apply_Y_kernel<<<blocks, threads>>>(data, num_states, target);
}

__global__ void apply_Z_kernel(double* data, size_t num_states, int target) {
    size_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= num_states / 2) return;

    double2* c_data = reinterpret_cast<double2*>(data);

    size_t stride = 1ULL << target;
    size_t block = tid / stride;
    size_t offset = tid % stride;

    size_t idx = (block * 2 * stride) + offset + stride;
    c_data[idx].x = -c_data[idx].x;
    c_data[idx].y = -c_data[idx].y;
}
void launch_apply_Z(double *data, size_t num_states, int target, int blocks, int threads) {
    apply_Z_kernel<<<blocks, threads>>>(data, num_states, target);
}

__global__ void apply_H_kernel(double* data, size_t num_states, int target) {
    size_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid >= num_states / 2) return;

    double2* c_data = reinterpret_cast<double2*>(data);

    size_t stride = 1ULL << target;
    size_t block = tid / stride;
    size_t offset = tid % stride;

    size_t idx1 = (block * 2 * stride) + offset;
    size_t idx2 = idx1 + stride;

    double2 val1 = c_data[idx1];
    double2 val2 = c_data[idx2];

    c_data[idx1].x = (val1.x + val2.x) * M_SQRT1_2;
    c_data[idx1].y = (val1.y + val2.y) * M_SQRT1_2;
    c_data[idx2].x = (val1.x - val2.x) * M_SQRT1_2;
    c_data[idx2].y = (val1.y - val2.y) * M_SQRT1_2;
}
void launch_apply_H(double *data, size_t num_states, int target, int blocks, int threads) {
    apply_H_kernel<<<blocks, threads>>>(data, num_states, target);
}
