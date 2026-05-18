//
// Created by cesare on 5/18/26.
//

#ifndef QUANTC_CNOTGATE_H
#define QUANTC_CNOTGATE_H


#include "Gates/TwoQubitGate.h"
#include <cmath>

class CNOTGate : public TwoQubitGate {
public:
    CNOTGate()  { name = "CNOT"; }
    void apply(QuantumState& qs, size_t control, size_t target) const override;
};


#endif //QUANTC_CNOTGATE_H
