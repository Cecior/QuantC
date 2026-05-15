//
// Created by cesare on 5/15/26.
//

#include <random>
#include "QuantumState.h"

QuantumState::QuantumState(int n) {
    numQubits = n;

    size_t size = 1 << numQubits;
    state.assign(size, Complex(0,0));
    state[0] = Complex(1,0);
}

size_t QuantumState::size() {
    return state.size();
}

Complex& QuantumState::operator[](size_t idx) {
    return state[idx];
}

void QuantumState::print_state() {
    std::cout << "|φ⟩ (" << numQubits << " qubits): ";
    for(size_t i = 0; i < state.size(); i++) {
        std::cout << state[i] << " ";
    }
    std::cout << std::endl;
}

int QuantumState::getNumQubits() {
    return numQubits;
}

int QuantumState::measure(size_t target) {
    size_t stride = 1ULL << target;
    double P1 = 0.0, P0 = 0.0;

    for(size_t block = 0; block < state.size(); block += stride * 2) {
        for(size_t offset = 0; offset < stride; offset++) {
            size_t idx1 = block | offset;
            size_t idx2 = idx1 | stride;

            Complex v1 = state[idx1];
            Complex v2 = state[idx2];

            P0 += norm(v1);
            P1 += norm(v2);
        }
    }

    std::cout << "Probability of qubit " << target << " in |0⟩: " << P0 * 100 << "%" << std::endl;
    std::cout << "Probability of qubit " << target << " in |1⟩: " << P1 * 100 << "%" << std::endl;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    double r = dist(gen);
    int result = (r < P0/(P0 + P1)) ? 0 : 1;
    double p_res = (result == 0) ? sqrt(P0) : sqrt(P1);
    size_t result_bit = result << target;

    std::cout << "Qubit " << target << "collapses to " << result << std::endl;

    if(p_res < 1e-15) {
        std::cout << "Probability near 0!" << std::endl;
        return -1;
    }

    double scale = 1.0 / p_res;

    for(size_t block = 0; block < state.size(); block += stride * 2) {
        for(size_t offset = 0; offset < stride; offset++) {
            size_t idx_one = block | offset | result_bit;
            size_t idx_zero = block| offset | (result_bit ^ stride);

            state[idx_one] *= scale;
            state[idx_zero] = Complex(0,0);
        }
    }

    return result;
}

double QuantumState::getAmplitude() {
    double amp = 0.0;

    for(size_t idx = 0; idx < state.size(); idx++) {
        amp += norm(state[idx]);
    }

    return sqrt(amp);
}
