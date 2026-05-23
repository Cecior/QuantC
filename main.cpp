//
// Created by cesare on 5/15/26.
//

#include <memory>
#include "benchmark.h"
#include "QuantumState.h"
#include "Gates.h"

int main() {
    std::unique_ptr<Gate> gate;
    gate = std::make_unique<ZGate>();
    benchmark::benchmark_gate(28, *gate, 0, 10);

    return 0;
}