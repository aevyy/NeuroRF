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


    return 0;
}