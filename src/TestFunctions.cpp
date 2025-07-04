#include "../include/NeuroRF/FeatureExtractor.hpp"
#include "../include/NeuroRF/SignalGenerator.hpp"
#include "../include/TestFunctions.hpp"

#include <utility>
#include <iostream>

using namespace std;

void testNoise() {
    NeuroRF::SignalGenerator generator;
    NeuroRF::FeatureExtractor extractor;
    
    // A clean BPSK signal
    std::vector<int> bits = {0, 1, 0, 1};
    std::vector<std::complex<double>> clean_signal;
    for (int bit : bits) {
        clean_signal.push_back(generator.generateBPSK(bit));
    }
    
    std::vector<std::complex<double>> noisy_signal = generator.addNoise(clean_signal, 0.1);
    
    std::vector<double> clean_features = extractor.basicFeatures(clean_signal);
    std::vector<double> noisy_features = extractor.basicFeatures(noisy_signal);
    
    std::cout << "Clean BPSK Q variance: " << clean_features[4] << std::endl;
    std::cout << "Noisy BPSK Q variance: " << noisy_features[4] << std::endl;
}

void testFeatureExtraction() {
    NeuroRF::SignalGenerator    generator;
    NeuroRF::FeatureExtractor   extractor;

    // Test 1: BPSK test
    std::vector<std::complex<double>> bpsk_signals;
    std::vector<int> bpsk_bits = {0, 1, 1, 0, 1, 0, 0, 1, 0, 1};

    for (auto bits : bpsk_bits) {
        std::complex<double> signal = generator.generateBPSK(bits);
        bpsk_signals.push_back(signal);
    }

    std::vector<double> bpskFeatures = extractor.basicFeatures(bpsk_signals);

    double bpsk_i_mean       = bpskFeatures[0];
    double bpsk_i_var        = bpskFeatures[1];
    double bpsk_i_stdDev     = bpskFeatures[2];

    double bpsk_q_mean       = bpskFeatures[3];
    double bpsk_q_var        = bpskFeatures[4];
    double bpsk_q_stdDev     = bpskFeatures[5];
    
    cout << "BPSK I values mean: " << bpsk_i_mean << endl;
    cout << "BPSK I values variance: " << bpsk_i_var << endl;
    cout << "BPSK I values std dev: " << bpsk_i_stdDev << endl;

    cout << "====================" << endl;

    cout << "BPSK Q values mean: " << bpsk_q_mean << endl;
    cout << "BPSK Q values variance: " << bpsk_q_var << endl;
    cout << "BPSK Q values std dev: " << bpsk_q_stdDev << endl;

    cout << "--------------------" << endl;

    // Test 2: QPSK test
    std::vector<std::complex<double>> qpsk_signals;
    std::vector<pair<int,int>> qpsk_bits = {{1, 0}, {1, 1}, {0, 0}, {1, 0}, {0, 1}, {1, 1}};

    for (auto bits : qpsk_bits) {
        std::complex<double> qpsk_signal = generator.generateQPSK(bits.first, bits.second);
        qpsk_signals.push_back(qpsk_signal);
    }

    std::vector<double> qpskFeatures = extractor.basicFeatures(qpsk_signals);

    double qpsk_i_mean       = qpskFeatures[0];
    double qpsk_i_var        = qpskFeatures[1];
    double qpsk_i_stdDev     = qpskFeatures[2];

    double qpsk_q_mean       = qpskFeatures[3];
    double qpsk_q_var        = qpskFeatures[4];
    double qpsk_q_stdDev     = qpskFeatures[5];
    
    cout << "qpsk I values mean: " << qpsk_i_mean << endl;
    cout << "qpsk I values variance: " << qpsk_i_var << endl;
    cout << "qpsk I values std dev: " << qpsk_i_stdDev << endl;

    cout << "====================" << endl;

    cout << "qpsk Q values mean: " << qpsk_q_mean << endl;
    cout << "qpsk Q values variance: " << qpsk_q_var << endl;
    cout << "qpsk Q values std dev: " << qpsk_q_stdDev << endl;

    cout << "--------------------" << endl;
}