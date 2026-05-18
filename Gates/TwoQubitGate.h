//
// Created by cesare on 5/18/26.
//

#ifndef QUANTC_TWOQUBITGATE_H
#define QUANTC_TWOQUBITGATE_H


#include "Gate.h"

class TwoQubitGate : public Gate {
public:
    TwoQubitGate(std::string gateName) : Gate(std::move(gateName)) {}
    void apply(QuantumState& qs, int control, int target) const override = 0;
};


#endif //QUANTC_TWOQUBITGATE_H
