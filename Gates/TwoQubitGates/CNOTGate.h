//
// Created by cesare on 5/18/26.
//

#ifndef QUANTC_CNOTGATE_H
#define QUANTC_CNOTGATE_H


#include "Gates/TwoQubitGate.h"
#include <math.h>

class CNOTGate : public TwoQubitGate {
public:
    CNOTGate(std::string gateName) : TwoQubitGate(std::move(gateName)) {}
    void apply(QuantumState& qs, int control, int target) const override;
};


#endif //QUANTC_CNOTGATE_H
