//
// Created by cesare on 5/18/26.
//

#ifndef QUANTC_TWOQUBITGATE_H
#define QUANTC_TWOQUBITGATE_H


#include "Gate.h"

class TwoQubitGate : public Gate {
public:
    void apply(QuantumState& qs, size_t control, size_t target) const override = 0;
};


#endif //QUANTC_TWOQUBITGATE_H
