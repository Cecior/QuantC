//
// Created by cesare on 5/15/26.
//

#include "QuantumState.h"
#include "Gates/XGate/XGate.h"

int main() {
    int n = 3;
    QuantumState qs = QuantumState(n);

    qs.print_state();
    std::cout << "|φ⟩ amplitude: " << qs.getAmplitude() << std::endl;

    XGate xGate;
    xGate.apply(qs, 1);

    qs.print_state();
    std::cout << "|φ⟩ amplitude: " << qs.getAmplitude() << std::endl;

    return 0;
}