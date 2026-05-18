//
// Created by cesare on 5/18/26.
//

#include "CNOTGate.h"

void CNOTGate::apply(QuantumState &qs, size_t control, size_t target) const {
    if(control == target) {
        throw std::runtime_error("Control and Target qubit cannot be the same!");
    }

    size_t control_bit = 1ULL << control;
    size_t target_bit = 1ULL << target;

    size_t low_stride = 1ULL << std::min(control, target);
    size_t high_stride = 1ULL << std::max(control, target);

    for(size_t high = 0; high < qs.size(); high += high_stride * 2) {
        for(size_t med = 0; med < high_stride; med += low_stride * 2) {
            for(size_t low = 0; low < low_stride; low++) {
                size_t base = high | med | low | control_bit;

                size_t idx1 = base;
                size_t idx2 = base | target_bit;

                std::swap(qs[idx1], qs[idx2]);
            }
        }
    }
}
