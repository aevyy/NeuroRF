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
    testBatchSignalGeneration();

    // CSV generation test
    NeuroRF::SignalGenerator generator;
    generator.generateTrainingCSV("rf_training_sample.csv", 1000);

    return 0;
}