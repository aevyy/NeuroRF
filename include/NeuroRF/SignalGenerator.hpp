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
    BPSK, QPSK, MOD_8PSK,
    MOD_16QAM, MOD_64QAM, 
    AM, FM,
    GMSK, FSK, ASK, PAM
};

// struct Triplets {   // Primarily to hold the triplets of 8-PSK bits
//     int bit1;
//     int bit2;
//     int bit3;
// };

class SignalGenerator {
public:
SignalGenerator();

std::complex<double> generateBPSK(int bit);
std::complex<double> generateQPSK(int bit1, int bit2);
std::complex<double> generate8PSK(int bit1, int bit2, int bit3);

std::vector<std::complex<double>> generateBPSKSequence(const std::vector<int> &bits);
std::vector<std::complex<double>> generateQPSKSequence(const std::vector<std::pair<int,int>> &bits);
std::vector<std::complex<double>> generate8PSKSequence(const std::vector<int> &bits);

void generateTrainingCSV(int samplesPerClass);

std::vector<std::complex<double>> addNoise(
    const std::vector<std::complex<double>> &signal,
    double noisePeak
);

private:
// Random components to generate noise
std::mt19937 generator;
std::random_device rd;
};

}   // Namespace NeuroRF