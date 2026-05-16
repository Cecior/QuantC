//
// Created by cesare on 5/16/26.
//

#ifndef QUANTC_XGATE_H
#define QUANTC_XGATE_H


#include "QuantumState.h"
#include "Gates/OneQubitGate.h"

class XGate : public OneQubitGate {
public:
    XGate(std::string gateName) : OneQubitGate(std::move(gateName)) {}
    void apply(QuantumState& qs, int target) const override;
};


#endif //QUANTC_XGATE_H
