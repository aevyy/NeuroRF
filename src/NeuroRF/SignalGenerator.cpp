#include "..\..\include\NeuroRF\SignalGenerator.hpp"

// Constructor: initializes the class with random generator & random seed
NeuroRF::SignalGenerator::SignalGenerator() : generator(rd()) {}


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


/* 
Ref: https://en.wikipedia.org/wiki/Independent_and_identically_distributed_random_variables

Principle:
'''
In probability theory and statistics, a collection of random variables
is independent and identically distributed (i.i.d., iid, or IID) if each
random variable has the same probability distribution as the others and
all are mutually independent. IID was first defined in statistics and
finds application in many fields, such as data mining and signal processing.
'''
*/
std::vector<std::complex<double>> NeuroRF::SignalGenerator::addNoise(
    const std::vector<std::complex<double>> &signal,
    double noisePeak) {
        std::normal_distribution<double> noise(0.0, noisePeak);
        std::vector<std::complex<double>> noisySignal;

        for (const auto &sample : signal) {
            double i_noise = noise(generator);
            double q_noise = noise(generator);

            noisySignal.push_back(sample + std::complex<double> (i_noise, q_noise));
        }

        return noisySignal;
    }


std::vector<std::complex<double>> NeuroRF::SignalGenerator::generateBPSKSequence(const std::vector<int> &bits) {
    std::vector<std::complex<double>> signals;

    for (const auto &bit : bits) {
        signals.push_back(generateBPSK(bit));
    }

    return signals;
}

std::vector<std::complex<double>> NeuroRF::SignalGenerator::generateQPSKSequence(const std::vector<std::pair<int,int>> &bits) {
    std::vector<std::complex<double>> signals;

    for (const auto &bitPair : bits) {
        signals.push_back(generateQPSK(bitPair.first, bitPair.second));
    }

    return signals;
}