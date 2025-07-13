#include "..\..\..\include\NeuroRF\SignalGenerator.hpp"

/* --- Constructor --- */
NeuroRF::SignalGenerator::SignalGenerator() : generator(rd()) {}

/* --- Noise Adder --- */
std::vector<std::complex<double>> NeuroRF::SignalGenerator::addNoise(
    const std::vector<std::complex<double>> &signal,
    double noisePeak) {
        std::normal_distribution<double> noise(0.0, noisePeak);
        std::vector<std::complex<double>> noisySignal;

        for (const auto &sample : signal) {
            double i_noise = noise(this->generator);
            double q_noise = noise(this->generator);

            noisySignal.push_back(sample + std::complex<double> (i_noise, q_noise));
        }

        return noisySignal;
    }