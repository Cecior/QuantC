//
// Created by cesare on 7/27/26.
//

#include <iostream>
#include <cuda_runtime.h>

int main() {
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);

    std::cout << "Name: " << prop.name << std::endl;
    std::cout << "Compute capability: " << prop.major << "." << prop.minor << std::endl;
    std::cout << "Multiprocessors (SM): " << prop.multiProcessorCount << std::endl;

    int cudaCores = prop.multiProcessorCount * 128;
    std::cout << "Estimated CUDA Cores: " << cudaCores << std::endl;

    std::cout << "VRAM (GB): " << prop.totalGlobalMem / (1024.0 * 1024.0 * 1024.0) << std::endl;
    std::cout << "L2 Cache Size (MiB): " << prop.l2CacheSize / (1024.0 * 1024.0) << std::endl;
    std::cout << "Memory Bus Width (bits): " << prop.memoryBusWidth << std::endl;
    std::cout << "Max threads per block: " << prop.maxThreadsPerBlock << std::endl;

    return 0;
}