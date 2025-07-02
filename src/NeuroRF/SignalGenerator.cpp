#include "..\..\include\NeuroRF\SignalGenerator.hpp"

std::complex<double> NeuroRF::SignalGenerator::generateBPSK(int bit) {
    if (bit != 0 && bit != 1) {
        throw std::invalid_argument("Bit must be either 0 or 1");
    }

    if (bit == 0) return std::complex<double>(+1.0, 0.0);
    else return std::complex<double>(-1.0, 0.0);
}

std::complex<double> NeuroRF::SignalGenerator::generateQPSK(int bit1, int bit2) {
    if ((bit1 != 0 && bit1 != 1) || (bit2 != 0 && bit2 != 1)) {
        throw std::invalid_argument("Bits must be either 0 or 1");
    }
    
    if (bit1 == 0 && bit2 == 0) {
        return std::complex<double>(+1.0, +1.0);
    }

    else if (bit1 == 0 && bit2 == 1) {
        return std::complex<double>(-1.0, +1.0);
    }

    else if (bit1 == 1 && bit2 == 0) {
        return std::complex<double>(-1.0, -1.0);
    }

    else {
        return std::complex<double>(+1.0, -1.0);
    }
}