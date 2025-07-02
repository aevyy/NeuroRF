#pragma once

#include <vector>
#include <complex>
#include <string>
#include <random>

namespace NeuroRF {
    enum ModulationType {
        BPSK,
        QPSK,
        MOD_16_QAM
    };

    class SignalGenerator {
    public:
    std::complex<double> generateBPSK(int bit);
    std::complex<double> generateQPSK(int bit1, int bit2);

    private:

    };
}