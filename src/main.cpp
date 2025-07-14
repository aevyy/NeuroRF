
#include "../include/NeuroRF/SignalGenerator.hpp"
#include "../include/RFTrainer.hpp"
#include "../include/TestFunctions.hpp"
#include <iostream>
#include <filesystem>

int main() {
    NeuroRF::RFTrainer trainer;
    NeuroRF::SignalGenerator generator;
    generator.generateTrainingCSV(3000);

    // Train the model
    trainer.train("trainingData.csv", 50);

    // Test the model
    double test_accuracy = trainer.test("testingData.csv");
    std::cout << "Test accuracy: " << test_accuracy * 100 << "%" << std::endl;

    // Validate the model
    double val_accuracy = trainer.validate("validationData.csv");
    std::cout << "Validation accuracy: " << val_accuracy * 100 << "%" << std::endl;


    // testFeatureExtraction();

    return 0;
}