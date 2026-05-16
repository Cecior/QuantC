//
// Created by cesare on 5/16/26.
//

#ifndef QUANTC_ONEQUBITGATE_H
#define QUANTC_ONEQUBITGATE_H


#include "QuantumState.h"
#include "Gate.h"

class OneQubitGate : public Gate {
public:
    OneQubitGate(std::string gateName) : Gate(std::move(gateName)) {}
    void apply(QuantumState& qs, int target) const override = 0;
};


#endif //QUANTC_ONEQUBITGATE_H
