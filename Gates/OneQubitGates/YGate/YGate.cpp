//
// Created by cesare on 5/16/26.
//

#include "YGate.h"

void YGate::apply(QuantumState &qs, int target) const {
    size_t stride = 1ULL << target;

    for(size_t block = 0; block < qs.size(); block += stride * 2) {
        for(size_t offset = 0; offset < stride; offset++) {
            size_t idx1 = block | offset;
            size_t idx2 = block | offset | stride;

            Complex v1 = qs[idx1];
            Complex v2 = qs[idx2];

            qs[idx1] = Complex(v2.imag(), -v2.real());
            qs[idx2] = Complex (-v1.imag(), v1.real());
        }
    }
}
