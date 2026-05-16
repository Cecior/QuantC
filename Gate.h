//
// Created by cesare on 5/16/26.
//

#ifndef QUANTC_GATE_H
#define QUANTC_GATE_H


#include "QuantumState.h"

class Gate {
public:
    virtual void apply(QuantumState qs) = 0;
    virtual void apply_obs(QuantumState qs) = 0;
};


#endif //QUANTC_GATE_H
