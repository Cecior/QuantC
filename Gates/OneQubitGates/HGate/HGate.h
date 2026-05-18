//
// Created by cesare on 5/16/26.
//

#ifndef QUANTC_HGATE_H
#define QUANTC_HGATE_H


#include "QuantumState.h"
#include "Gates/OneQubitGate.h"

class HGate : public OneQubitGate {
public:
    HGate() { name = "Hadamard"; };
    void apply(QuantumState& qs, size_t target) const override;
};


#endif //QUANTC_HGATE_H
