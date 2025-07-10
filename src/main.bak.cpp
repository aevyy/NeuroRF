
// #include "../include/NeuroRF/SignalGenerator.hpp"
// #include "../include/RFTrainer.hpp"
// #include "../include/TestFunctions.hpp"
// #include <iostream>
// #include <filesystem>

// int main() {
//     try {
//         // Always generate training data (overwrite existing file)
//         std::cout << "Generating RF training data..." << std::endl;
//         NeuroRF::SignalGenerator generator;
//         generator.generateTrainingCSV(10000);

//         // Training RF classifier
//         std::cout << "Training RF classifier..." << std::endl;
//         NeuroRF::RFTrainer trainer;
//         trainer.train("trainingData.csv", 1000);

//         // Testing RF classifier
//         std::cout << "Testing RF classifier..." << std::endl;
//         double testAccuracy = trainer.test("testingData.csv");
//         std::cout << "Accuracy: " << testAccuracy * 100 << "%" << std::endl;

//         // Validating RF classifier
//         std::cout << "\nValidating RF classifier..." << std::endl;
//         double validationAccuracy = trainer.validate("validationData.csv");
//         std::cout << "Validation completed!" << std::endl;

//         std::cout << "\n === SUMMARY === " << std::endl;
//         std::cout << "Test Accuracy:        " << testAccuracy * 100 << "%" << std::endl;
//         std::cout << "Validation Accuracy   " << validationAccuracy * 100 << "%" << std::endl;

//     } catch (const std::exception& e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return 1;
//     }

//     return 0;
// }