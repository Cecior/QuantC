//
// Created by cesare on 5/15/26.
//

#include <memory>
#include "QuantumState.h"
#include "Gates/OneQubitGates/XGate/XGate.h"
#include "Gates/OneQubitGates/ZGate/ZGate.h"
#include "Gates/OneQubitGates/HGate/HGate.h"
#include "Gates/OneQubitGates/YGate/YGate.h"

int benchmark() {
    int n = 3;
    QuantumState qs = QuantumState(n);

    qs.print_state();
    std::cout << "|φ⟩ amplitude: " << qs.getAmplitude() << std::endl;

    std::unique_ptr<Gate> gate;

    gate=std::make_unique<XGate>();
    gate->apply(qs, 0);

    qs.print_state();
    std::cout << "|φ⟩ amplitude: " << qs.getAmplitude() << std::endl;

    return 0;
}