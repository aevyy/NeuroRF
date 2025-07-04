#include "../include/NeuroRF/SignalGenerator.hpp"
#include "../include/NeuroRF/FeatureExtractor.hpp"
#include "../include/TestFunctions.hpp"
#include <iostream>

using namespace std;

void testFeatureExtraction();
void testNoise();

int main() {
    testFeatureExtraction();
    testNoise();

    // Test generateBPSKsequence, no5t making seperate function for this one
    NeuroRF::SignalGenerator generator;
    vector<int> test_bits = {0, 1, 0, 1, 0, 1};
    vector<std::complex<double>> batchSignal = generator.generateBPSKSequence(test_bits);
    cout << "Generated " << batchSignal.size() << " BPSK symbols" << endl;
    for(auto i : batchSignal) cout << i << "\t"; 
    

    return 0;
}