#include "..\..\..\include\NeuroRF\SignalGenerator.hpp"

/*---- BPSK ....*/
std::complex<double> NeuroRF::SignalGenerator::generateBPSK(int bit) {
    if (bit != 0 && bit != 1) {
        throw std::invalid_argument("Bit must be either 0 or 1");
    }

    if (bit == 0) return std::complex<double>(+1.0, 0.0);
    else return std::complex<double>(-1.0, 0.0);
}

std::vector<std::complex<double>> NeuroRF::SignalGenerator::generateBPSKSequence(const std::vector<int> &bits) {
    std::vector<std::complex<double>> signals;

    for (const auto &bit : bits) {
        signals.push_back(this->generateBPSK(bit));
    }

    return signals;
}

/*--- QPSK ---*/
std::complex<double> NeuroRF::SignalGenerator::generateQPSK(int bit1, int bit2) {
    if ((bit1 != 0 && bit1 != 1) || (bit2 != 0 && bit2 != 1)) {
        throw std::invalid_argument("Bits must be either 0 or 1");
    }
    
    // Normalizing by sqrt(2) to have same power as BPSK
    double scale = 1.0 / std::sqrt(2.0);
    
    if (bit1 == 0 && bit2 == 0) {
        return std::complex<double>(+1.0 * scale, +1.0 * scale);
    }
    else if (bit1 == 0 && bit2 == 1) {
        return std::complex<double>(-1.0 * scale, +1.0 * scale);
    }
    else if (bit1 == 1 && bit2 == 0) {
        return std::complex<double>(-1.0 * scale, -1.0 * scale);
    }
    else {
        return std::complex<double>(+1.0 * scale, -1.0 * scale);
    }
}

std::vector<std::complex<double>> NeuroRF::SignalGenerator::generateQPSKSequence(const std::vector<std::pair<int,int>> &bits) {
    std::vector<std::complex<double>> signals;

    for (const auto &bitPair : bits) {
        signals.push_back(generateQPSK(bitPair.first, bitPair.second));
    }

    return signals;
}

/* --- 8PSK ---*/
std::complex<double> NeuroRF::SignalGenerator::generate8PSK(int bit1, int bit2, int bit3) {
    /*
    My strategy to generate the respective signals as per  the bits
    combinations is to first convert the bits combo to its respective decimal
    form, i.e. 0-7 (binary 111, which is the max we can get as argument,
    gives 7). Then it gets easier to get the respective signals using an
    expression:  angle = 2 * pi * k/8   
    */

    if ((bit1 != 0 && bit1 != 1) || (bit2 != 0 && bit2 != 1) || (bit3 != 0 && bit3 != 1)) {
        throw std::invalid_argument("Bits must be either 1 or 0");
    }

    // First, lets convert the bits' combo to decimals (0-7)
    int dec = (bit1 << 2) | (bit2 << 1) | (bit3);    // == (bit1 * 4) + (bit2 * 2) + bit3
    double angle = 2.0 * M_PI * dec / 8.0;
    return std::polar(1.0, angle); // Gives the complex number for radius 1 and the given angle
}

std::vector<std::complex<double>> NeuroRF::SignalGenerator::generate8PSKSequence(const std::vector<int> &bits) {
    std::vector<std::complex<double>> signals;
    NeuroRF::SignalGenerator generator;
    for (int i = 0; i < bits.size() - 2; i += 3) {
        signals.push_back(generator.generate8PSK(bits[i], bits[i+1], bits[i+2]));
    }

    return signals;
}