//
// Created by cesare on 5/16/26.
//

#ifndef QUANTC_GATE_H
#define QUANTC_GATE_H


#include <utility>

#include "QuantumState.h"

class Gate {
private:
    std::string name;
public:
    Gate(std::string gateName) : name(std::move(gateName)) {}

    virtual void apply(QuantumState& qs, int target) const {
        throw std::runtime_error(name + " is not a 1-qubit gate!");
    };

    virtual void apply(QuantumState& qs, int control, int target) const {
        throw std::runtime_error(name + " is not a 2-qubit gate!");
    };
    
    //virtual void apply_obs(QuantumState& qs) const = 0;
    std::string getName() const { return name; }
};


#endif //QUANTC_GATE_H
