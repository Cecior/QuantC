//
// Created by cesare on 5/16/26.
//

#include "HGate.h"

void HGate::apply(QuantumState &qs, size_t target) const {
    size_t stride = 1ULL << target;

    double c = 1.0 / sqrt(2);

    for(size_t block = 0; block < qs.size(); block += stride * 2) {
        for(size_t offset = 0; offset < stride; offset++) {
            size_t idx1 = block | offset;
            size_t idx2 = block | offset | stride;

            Complex v1 = qs[idx1];
            Complex v2 = qs[idx2];

            qs[idx1] = c*(v1+v2);
            qs[idx2] = c*(v1-v2);
        }
    }
}
