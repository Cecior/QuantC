//
// Created by cesare on 5/15/26.
//

#include "kernel_cpu.h"
#include <iostream>
#include <chrono>

#define CHECK_CUDA(Call) { \
    cudaError_t err = Call; \
    if (err != cudaSuccess) { \
        std::cerr << "CUDA Error in " << __FILE__ << " line " << __LINE__ << " - " << cudaGetErrorString(err) << std::endl; \
        exit(EXIT_FAILURE); \
    } \
} \


double* aligned_state_vector(int nq) {
    size_t n_elem = 1ULL << (nq + 1);
    size_t size_in_bytes = n_elem * sizeof(double);
    double* data = static_cast<double*>(aligned_alloc(32, size_in_bytes));

    for (int i = 0; i < n_elem; i++) {
        data[i] = 0.0;
    }
    data[0] = 1.0;

    return data;
}
void benchmark_cpu(int num_qubit, int iter) {
    size_t n_elem = 1ULL << (num_qubit + 1);
    
    for(int target = 0; target < num_qubit; target++) {
        std::cout << "Qubit #" << target << std::endl;
        double* qs = aligned_state_vector(num_qubit);

        auto start = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < iter; i++) {
            apply_Y_AVX(qs, n_elem, target);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        double time = duration.count() / iter;

        free(qs);
        std::cout << "Avg time: " << time * 1000 << " ms" << std::endl;
    }
}

__global__ void apply_X_kernel(double* data, size_t num_states, int target) {
    size_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid > num_states / 2) return;

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

__global__ void apply_Y_kernel(double* data, size_t num_states, int target) {
    size_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid > num_states / 2) return;

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

__global__ void apply_Z_kernel(double* data, size_t num_states, int target) {
    size_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid > num_states / 2) return;

    double2* c_data = reinterpret_cast<double2*>(data);

    size_t stride = 1ULL << target;
    size_t block = tid / stride;
    size_t offset = tid % stride;

    size_t idx = (block * 2 * stride) + offset + stride;
    c_data[idx].x = -c_data[idx].x;
    c_data[idx].y = -c_data[idx].y;
}

void benchmark_gpu(int num_qubit, int iter) {

    size_t num_states = 1ULL << num_qubit;
    size_t size_in_bytes = num_states * 2 * sizeof(double);

    // CPU Allocation
    double* h_qs = (double*)malloc(size_in_bytes);
    memset(h_qs, 0, size_in_bytes);
    h_qs[0] = 1.0;

    // GPU Allocation
    double* d_qs;
    CHECK_CUDA(cudaMalloc(&d_qs, size_in_bytes));
    CHECK_CUDA(cudaMemcpy(d_qs, h_qs, size_in_bytes, cudaMemcpyHostToDevice));

    size_t total_swaps = num_states / 2;
    int threads_per_block = 256;
    int blocks = (total_swaps + threads_per_block - 1) / threads_per_block;

    for (int target = 0; target < num_qubit; target++) {
        std::cout << "Qubit #" << target << ": " << std::endl;

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iter; i++) {
            apply_Y_kernel<<<blocks, threads_per_block>>>(d_qs, num_states, target);
            CHECK_CUDA(cudaGetLastError());
            CHECK_CUDA(cudaDeviceSynchronize());
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        double time = duration.count() / iter;

        std::cout << "Avg time: " << time << " ms" << std::endl;
    }

    CHECK_CUDA(cudaFree(d_qs));
    free(h_qs);
}

int main() {
    int n = 5;
    int nq = 27;

    benchmark_cpu(nq, n);
    return 0;
}