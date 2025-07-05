
#include "../include/NeuroRF/SignalGenerator.hpp"
#include "../include/RFTrainer.hpp"
#include "../include/TestFunctions.hpp"
#include <iostream>
#include <filesystem>

int main() {
    try {
        // Always generate training data (overwrite existing file)
        std::cout << "Generating RF training data..." << std::endl;
        NeuroRF::SignalGenerator generator;
        generator.generateTrainingCSV("rf_training.csv", 1000);

        // Training RF classifier
        std::cout << "Training RF classifier..." << std::endl;
        NeuroRF::RFTrainer trainer;
        trainer.train("rf_training.csv", 100);

        // Testing RF classifier
        std::cout << "Testing RF classifier..." << std::endl;
        double accuracy = trainer.test("rf_training.csv");
        std::cout << "Accuracy: " << accuracy * 100 << "%" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}