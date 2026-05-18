//
// Created by cesare on 5/16/26.
//

#ifndef QUANTC_GATE_H
#define QUANTC_GATE_H


#include <utility>

#include "QuantumState.h"

class Gate {
protected:
    std::string name;
public:
    virtual ~Gate() = default;

    virtual void apply(QuantumState& qs, size_t target) const {
        throw std::runtime_error(name + " is not a 1-qubit gate!");
    };

    virtual void apply(QuantumState& qs, size_t control, size_t target) const {
        throw std::runtime_error(name + " is not a 2-qubit gate!");
    };
    
    //virtual void apply_obs(QuantumState& qs) const = 0;
    std::string getName() const { return name; }
};


#endif //QUANTC_GATE_H
