//
// Created by cesare on 5/16/26.
//

#ifndef QUANTC_XGATE_H
#define QUANTC_XGATE_H


#include "QuantumState.h"

class XGate {
public:
    void apply(QuantumState& qs, int target);
};


#endif //QUANTC_XGATE_H
