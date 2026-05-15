//
// Created by cesare on 5/15/26.
//

#ifndef QUANTC_QUANTUMSTATE_H
#define QUANTC_QUANTUMSTATE_H

#include <vector>
#include <complex>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <utility>
#include <chrono>
#include <string>

using Complex = std::complex<double>;

class QuantumState {
private:
    int numQubits;
    std::vector<Complex> state;

public:
    QuantumState(int n);

    size_t size();
    Complex& operator[](size_t idx);

    void print_state();

    int getNumQubits();
    double getAmplitude();
    int measure(size_t target);
};

#endif //QUANTC_QUANTUMSTATE_H
