//
// Created by cesare on 5/16/26.
//

#ifndef QUANTC_ZGATE_H
#define QUANTC_ZGATE_H


#include "QuantumState.h"
#include "Gates/OneQubitGate.h"

class ZGate : public OneQubitGate{
public:
    ZGate() { name = "Z"; }
    void apply(QuantumState& qs, size_t target) const override;
};

#endif //QUANTC_ZGATE_H