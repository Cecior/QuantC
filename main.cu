//
// Created by cesare on 5/15/26.
//

#include "kernel_cpu.h"
#include "kernel_gpu.cuh"
#include <iostream>
#include <chrono>
#include <fstream>

#define LOG if(g_verbose) std::cout
#define SAVE(name_impl, target, time_ms) if(save) save_result(name_impl, target, time_ms)

#define CHECK_CUDA(Call) { \
    cudaError_t err = Call; \
    if (err != cudaSuccess) { \
        std::cerr << "CUDA Error in " << __FILE__ << " line " << __LINE__ << " - " << cudaGetErrorString(err) << std::endl; \
        exit(EXIT_FAILURE); \
    } \
} \

#define RES_PATH "../experiments/experiment_004/results.csv"

static bool g_verbose = false;
static bool save = false;
static volatile double g_sink = 0.0;

typedef void (*CpuGateFunction)(double*, size_t, int);
typedef void (*GpuGateFunction)(double*, size_t, int, int, int);

static void save_result(const std::string &name_impl, int target, double time_ms);
static double* aligned_state_vector(int nq);

static void benchmark_cpu(int num_qubit, int iter, CpuGateFunction gate, const std::string name, std::string str_target);
static void benchmark_gpu(int num_qubit, int iter, GpuGateFunction gate, const std::string name, std::string str_target);

int main(int argc, char* argv[]) {
    int iter = 5;
    int num_qubit = 27;

    std::string target = "all";
    std::string impl = "avx";
    CpuGateFunction cpuGate = &apply_H_AVX;
    GpuGateFunction gpuGate = &launch_apply_H;

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--verbose") g_verbose = true;
        if (std::string(argv[i]) == "--num_qubit" && i < argc - 1) num_qubit = std::stoi(argv[++i]);
        if (std::string(argv[i]) == "--iter" && i < argc - 1) iter = std::stoi(argv[++i]);
        if (std::string(argv[i]) == "--impl" && i < argc - 1) impl = std::string(argv[++i]);
        if (std::string(argv[i]) == "--target" && i < argc - 1) target = std::string(argv[++i]);
        if (std::string(argv[i]) == "--save") save = true;
    }

    if (impl == "raw") cpuGate = &apply_H_raw;
    else if (impl == "omp") cpuGate = &apply_H_omp;
    else if (impl == "avx") cpuGate = &apply_H_AVX;

    std::string str_name = "H - " + impl;

    if (impl == "cuda")
        benchmark_gpu(num_qubit, iter, gpuGate, str_name, target);
    else
        benchmark_cpu(num_qubit, iter, cpuGate, str_name, target);

    return 0;
}

void save_result(const std::string &name_impl, int target, double time_ms) {
    std::ifstream check(RES_PATH);
    bool exists = check.good();

    std::ofstream file(RES_PATH, std::ios::app);

    if (!file.is_open()) {
        std::cerr << "Cannot open file" << std::endl;
        return;
    }

    if (!exists) file << "name_impl, target, time_ms\n";

    file << name_impl << ", " << target << ", " << time_ms << std::endl;
}
double* aligned_state_vector(int nq) {
    size_t n_elem = 1ULL << (nq + 1);
    size_t size_in_bytes = n_elem * sizeof(double);

    #ifdef _MSC_VER
        double* data = static_cast<double*>(_aligned_malloc(size_in_bytes, 32));
    #else
        double* data = static_cast<double*>(aligned_alloc(32, size_in_bytes));
    #endif

    for (int i = 0; i < n_elem; i++) {
        data[i] = 0.0;
    }
    data[0] = 1.0;

    return data;
}
void benchmark_cpu(int num_qubit, int iter, CpuGateFunction gate, const std::string name, std::string str_target) {
    size_t num_states = 1ULL << num_qubit;
    size_t size = 2 * num_states;
    double* qs = aligned_state_vector(num_qubit);

    LOG << "Benchmarking Gate " << name << " on " << num_qubit << " qubits" << std::endl;

    int begin = (str_target == "all" || std::stoi(str_target) < 0 || std::stoi(str_target) >= num_qubit) ? 0 : std::stoi(str_target);
    int end = (str_target == "all" || std::stoi(str_target) < 0 || std::stoi(str_target) >= num_qubit) ? num_qubit : std::stoi(str_target) + 1;

    for(int target = begin; target < end; target++) {
        LOG << "Qubit #" << target << std::endl;

        auto t_start = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < iter; i++) {
            gate(qs, size, target);
        }
        auto t_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = t_end - t_start;
        double time = duration.count() / iter;

        SAVE(name, target, time);

        g_sink += qs[num_states / 2];

        LOG << "Avg time: " << time << " ms" << std::endl;
    }
    #ifdef _MSC_VER
        _aligned_free(qs);
    #else
        free(qs);
    #endif
}
void benchmark_gpu(int num_qubit, int iter, GpuGateFunction gate, const std::string name, std::string str_target) {
    size_t num_states = 1ULL << num_qubit;
    size_t size_in_bytes = num_states * 2 * sizeof(double);

    // CPU Allocation
    double* h_qs = aligned_state_vector(num_qubit);
    memset(h_qs, 0, size_in_bytes);
    h_qs[0] = 1.0;

    // GPU Allocation
    double* d_qs;
    CHECK_CUDA(cudaMalloc(&d_qs, size_in_bytes));
    CHECK_CUDA(cudaMemcpy(d_qs, h_qs, size_in_bytes, cudaMemcpyHostToDevice));

    size_t total_swaps = num_states / 2;
    int threads_per_block = 256;
    int blocks = (total_swaps + threads_per_block - 1) / threads_per_block;

    LOG << "Benchmarking Gate " << name << " on " << num_qubit << " qubits" << std::endl;

    int begin = (str_target == "all" || std::stoi(str_target) < 0 || std::stoi(str_target) >= num_qubit) ? 0 : std::stoi(str_target);
    int end = (str_target == "all" || std::stoi(str_target) < 0 || std::stoi(str_target) >= num_qubit) ? num_qubit : std::stoi(str_target) + 1;

    for (int target = begin; target < end; target++) {
        LOG << "Qubit #" << target << ": " << std::endl;

        auto t_start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iter; i++) {
            gate(d_qs, num_states, target, blocks, threads_per_block);
            CHECK_CUDA(cudaGetLastError());
        }
        CHECK_CUDA(cudaDeviceSynchronize());
        auto t_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = t_end - t_start;
        double time = duration.count() / iter;

        SAVE(name, target, time);

        LOG << "Avg time: " << time << " ms" << std::endl;
    }

    CHECK_CUDA(cudaFree(d_qs));

    #ifdef _MSC_VER
        _aligned_free(h_qs);
    #else
        free(h_qs);
    #endif
}