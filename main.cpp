//
// Created by cesare on 5/15/26.
//

#include "QuantumState.h"
#include "Gates/OneQubitGates/XGate/XGate.h"
#include "Gates/OneQubitGates/ZGate/ZGate.h"
#include "Gates/OneQubitGates/HGate/HGate.h"
#include "Gates/OneQubitGates/YGate/YGate.h"

int main() {
    int n = 3;
    QuantumState qs = QuantumState(n);

    qs.print_state();
    std::cout << "|φ⟩ amplitude: " << qs.getAmplitude() << std::endl;

    YGate gate("X-Gate");
    gate.apply(qs, 0);

    qs.print_state();
    std::cout << "|φ⟩ amplitude: " << qs.getAmplitude() << std::endl;

    return 0;
}