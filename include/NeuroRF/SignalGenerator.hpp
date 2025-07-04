#pragma once

#include <vector>
#include <complex>
#include <string>
#include <random>
#include <fstream>
#include <iostream>
#include <utility>

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
    
    std::vector<std::complex<double>> generateBPSKSequence(const std::vector<int> &bits);
    std::vector<std::complex<double>> generateQPSKSequence(const std::vector<std::pair<int,int>> &bits);

    void generateTrainingCSV(const std::string &fiile, int samplesPerClass);

    std::vector<std::complex<double>> addNoise(
        const std::vector<std::complex<double>> &signal,
        double noisePeak
    );

    private:
    // Random components to generate noise
    std::mt19937 generator;
    std::random_device rd;
    };
}