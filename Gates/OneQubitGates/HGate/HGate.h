//
// Created by cesare on 5/16/26.
//

#ifndef QUANTC_HGATE_H
#define QUANTC_HGATE_H


#include "QuantumState.h"
#include "Gates/OneQubitGate.h"

class HGate : public OneQubitGate {
public:
    HGate(std::string gateName) : OneQubitGate(std::move(gateName)) {};
    void apply(QuantumState& qs, int target) const override;
};


#endif //QUANTC_HGATE_H
