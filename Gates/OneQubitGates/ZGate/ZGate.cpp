//
// Created by cesare on 5/16/26.
//

#include "ZGate.h"

void ZGate::apply(QuantumState& qs, int target) const {
    size_t stride = 1ULL << target;

    for(size_t block = 0; block < qs.size(); block += stride * 2) {
        for(size_t offset = 0; offset < stride; offset++) {
            size_t idx = block | offset | stride;

            qs[idx] *= -1;
        }
    }
}
