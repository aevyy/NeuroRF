#pragma once

#include <vector>
#include <complex>
#include <string>
#include <random>

namespace NeuroRF {
    enum ModulationType {
        BPSK,
        QPSK,
        MOD_16_QAM  // Will look at this and additional types later on
    };

    class SignalGenerator {
    public:
    SignalGenerator();

    std::complex<double> generateBPSK(int bit);
    std::complex<double> generateQPSK(int bit1, int bit2);

    private:
    // Random components to generate noise
    std::mt19937 generator;
    std::random_device rd;
    };
}