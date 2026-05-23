//
// Created by cesare on 5/23/26.
//

#ifndef QUANTC_BENCHMARK_H
#define QUANTC_BENCHMARK_H

#include "QuantumState.h"
#include "Gates/Gate.h"

namespace benchmark {

    void benchmark_gate(size_t numQ, const Gate &gate, size_t target, int iterations = 10) {
        QuantumState qs(numQ);
        double total_time = 0.0;

        std::cout << "---------------------------------------------" << std::endl;
        std::cout << "Benchmarking gate " << gate.getName() << " on qubit " << target << std::endl;
        std::cout << "State size: " << (double) (qs.size() * sizeof(Complex)) / (1ULL << 20) << " MB" << std::endl;
        std::cout << std::endl;

        gate.apply(qs, target);

        std::cout << "Progress [" << std::flush;
        for(int i = 0; i < iterations; i++) {
            std::cout << "-" << std::flush;
        }
        std::cout << "]\rProgress [" << std::flush;

        for (int i = 0; i < iterations; i++) {
            auto start = std::chrono::high_resolution_clock::now();
            gate.apply(qs, target);
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> duration = end - start;
            total_time += duration.count();

            std::cout << "#" << std::flush;
        }

        std::cout << "]" << std::endl;
        std::cout << "Avg time: " << total_time / iterations << " s" << std::endl;
    }
}

#endif //QUANTC_BENCHMARK_H
