#include "../include/NeuroRF/SignalGenerator.hpp"
#include "../include/RFTrainer.hpp"
#include "../include/TestFunctions.hpp"
#include <iostream>
#include <vector>

int main() {
    NeuroRF::RFTrainer trainer;
    NeuroRF::SignalGenerator generator;
    generator.generateTrainingCSV(3000);

    // Training the model
    std::cout << "Training neural network..." << std::endl;
    trainer.train("../trainingData.csv", 100);

    // Printing weight matrices (first few values)
    std::cout << "\n=== Weight Matrix Analysis ===" << std::endl;
    std::cout << "Input->First row sample:" << std::endl;
    Matrix* w0 = trainer.getNetwork().getWeightMatrix(0);
    for (int j = 0; j < 6; j++) {
        std::cout << w0->getValue(0, j) << " ";
    }
    std::cout << "..." << std::endl;

    std::cout << "\nOutput Layer Weight Matrix (12x3):" << std::endl;
    Matrix* w2 = trainer.getNetwork().getWeightMatrix(2);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << w2->getValue(i, j) << " ";
        }
        std::cout << std::endl;
    }

    // Test and validation
    std::cout << "\n=== Performance Metrics ===" << std::endl;
    double test_accuracy = trainer.test("../testingData.csv");
    double val_accuracy = trainer.validate("../validationData.csv");
    
    std::cout << "Final Training Error: " << trainer.getNetwork().getTotalError() << std::endl;
    std::cout << "Test Accuracy: " << test_accuracy * 100 << "%" << std::endl;
    std::cout << "Validation Accuracy: " << val_accuracy * 100 << "%" << std::endl;
    
    // Printing network parameters summary
    std::cout << "\n=== Network Parameters Summary ===" << std::endl;
    std::cout << "Total Weight Matrices: 3" << std::endl;
    std::cout << "Matrix 0 (Input->Hidden1): " << w0->getNumRows() << "x" << w0->getNumCols() << std::endl;
    Matrix* w1 = trainer.getNetwork().getWeightMatrix(1);
    std::cout << "Matrix 1 (Hidden1->Hidden2): " << w1->getNumRows() << "x" << w1->getNumCols() << std::endl;
    std::cout << "Matrix 2 (Hidden2->Output): " << w2->getNumRows() << "x" << w2->getNumCols() << std::endl;

    return 0;
}