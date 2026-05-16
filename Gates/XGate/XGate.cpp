//
// Created by cesare on 5/16/26.
//

#include "XGate.h"

void XGate::apply(QuantumState& qs, int target) {
    size_t stride = 1ULL << target;

    for(size_t block = 0; block < qs.size(); block += stride * 2) {
        for(size_t offset = 0; offset < stride; offset++) {
            size_t idx1 = block | offset;
            size_t idx2 = block | offset | stride;

            std::swap(qs[idx1], qs[idx2]);
        }
    }
}
