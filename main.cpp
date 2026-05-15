//
// Created by cesare on 5/15/26.
//

#include "QuantumState.h"

int main() {
    int n = 3;
    QuantumState qs = QuantumState(n);

    qs.print_state();
    std::cout << "|φ⟩ amplitude: " << qs.getAmplitude() << std::endl;

    return 0;
}