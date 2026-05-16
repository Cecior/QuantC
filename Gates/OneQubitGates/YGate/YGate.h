//
// Created by cesare on 5/16/26.
//

#ifndef QUANTC_YGATE_H
#define QUANTC_YGATE_H


#include "Gates/OneQubitGate.h"

class YGate : public OneQubitGate {
public:
    YGate(std::string gateName) : OneQubitGate(std::move(gateName)) {}
    void apply(QuantumState& qs, int target) const override;
};


#endif //QUANTC_YGATE_H
