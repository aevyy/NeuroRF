#include "../include/RFTrainer.hpp"
#include "../include/DataLoader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

namespace NeuroRF {

RFTrainer::RFTrainer() : network({24, 24, 12, 4}) {}

void RFTrainer::train(const std::string& trainFile, int epochs) {
    DataLoader loader;

    loader.loadData(trainFile);
    const auto& features = loader.getFeatures();
    const auto& labels   = loader.getLabels();
    
    // DEBUG: Print training setup
    std::cout << "Training on " << features.size() << " samples for " << epochs << " epochs" << std::endl;
    
    // Training loop
    for (int epoch = 0; epoch < epochs; epoch++) {
        for (size_t i = 0; i < features.size(); i++) {
            network.setCurrentInput(features[i]);
            network.feedForward();
            
            // Convert integer label to one-hot vector for training
            std::vector<double> one_hot(4, 0.0);
            if (labels[i] >= 0 && labels[i] <= 3) {
                one_hot[labels[i]] = 1.0;
            }
            
            // DEBUG: Print first few training examples from both classes
            if (epoch == 0 && (i < 5 || (i >= features.size()/2 && i < features.size()/2 + 5))) {
                std::cout << "Training sample " << i << ": label=" << labels[i]
                          << " -> one_hot=[" << one_hot[0] << ", " << one_hot[1] << ", " << one_hot[2] << ", " << one_hot[3] << "]" << std::endl;
            }
            
            network.setCurrentTarget(one_hot);
            network.setErrors();
            network.backPropagation();
        }
        if (epoch % 10 == 0) {
            std::cout << "Epoch " << epoch << "/" << epochs << std::endl;
        }
    }
}

double RFTrainer::test(const std::string& testFile) {
    DataLoader loader;

    loader.loadData(testFile);
    const auto& features    = loader.getFeatures();
    const auto& labels      = loader.getLabels();
    
    std::cout << "Testing on " << features.size() << " samples" << std::endl;
    
    int correct = 0;
    std::cout << features.size();

    network.setCurrentTarget(features[features.size() - 1]);
    int printCount[4] = {0, 0, 0, 0};

    for (size_t i = 0; i < features.size(); i++) {
        network.setCurrentInput(features[i]);
        network.feedForward();
        
        // output from last layer
        Matrix output_matrix = network.getActivatedNeuronMatrix(network.topologySize - 1);
        
        // Checking matrix dimensions before accessing
        int rows = output_matrix.getNumRows();
        int cols = output_matrix.getNumCols();
        
        // Determining prediction based on matrix shape
        int predicted = 0;
        double maxVal;
        if (rows == 4 && cols == 1) {
            maxVal = output_matrix.getValue(0, 0);
            for (int k = 1; k < 4; k++) {
                double val = output_matrix.getValue(k, 0);
                if (val > maxVal) {
                    maxVal = val;
                    predicted = k;
                }
            }


            if (printCount[labels[i]] < 5) {
                std::cout << "Sample " << i << ": outputs["
                        << output_matrix.getValue(0, 0) << ", "
                        << output_matrix.getValue(1, 0) << ", "
                        << output_matrix.getValue(2, 0) << ", "
                        << output_matrix.getValue(3, 0) << "] -> predicted="
                        << predicted << ", actual=" << labels[i] << std::endl;
                printCount[labels[i]]++;
            }
        } 

        // row vector
        else if (rows == 1 && cols == 4) {
            maxVal = output_matrix.getValue(0, 0);
            for (int k = 1; k < 4; ++k) {
                double val = output_matrix.getValue(0, k);
                if (val > maxVal) {
                    maxVal = val;
                    predicted = k;
                }
            }
            // first 10 and last 10 predictions
            if (i < 10 || i >= features.size() - 10) {
                std::cout << "Sample " << i << ": outputs["
                        << output_matrix.getValue(0, 0) << ", "
                        << output_matrix.getValue(0, 1) << ", "
                        << output_matrix.getValue(0, 2) << ", "
                        << output_matrix.getValue(0, 3) << "] -> predicted="
                        << predicted << ", actual=" << labels[i] << std::endl;
            }
        }

        else {
            // Unknown format, printing debug info
            std::cout << "Unexpected matrix dimensions: " << rows << "x" << cols << std::endl;
            continue;
        }
        
        if (i < labels.size() && predicted == labels[i]) {
            correct++;
        }
    }
    
    if (features.empty()) {
        return 0.0;
    }
    
    return (double)correct / features.size();
    
}

double RFTrainer::validate(const std::string &valiFile) {
    DataLoader loader;

    loader.loadData(valiFile);
    const auto& features    = loader.getFeatures();
    const auto& labels      = loader.getLabels();

    std::cout << "Validating on " << features.size() << " samples." << std::endl;

    int correct = 0;
    int printCount[4] = {0, 0, 0, 0};

    // Validating samples
    for (size_t i = 0; i < features.size(); i++) {
        // Setting input features to perform forward pass
        network.setCurrentInput(features[i]);
        network.feedForward();

        // Output from last layer
        Matrix outputMatrix = network.getActivatedNeuronMatrix(network.topologySize - 1);

        // Lets check matrix dimensions to handle different output formats
        int rows = outputMatrix.getNumRows();
        int cols = outputMatrix.getNumCols();


        int predicted = 0;
        double maxVal;

        if (rows == 4 && cols == 1) {
            maxVal = outputMatrix.getValue(0, 0);
            for (int k = 1; k < 4; ++k) {
                double val = outputMatrix.getValue(k, 0);
                if (val > maxVal) {
                    maxVal = val;
                    predicted = k;
                }
            }
            if (printCount[labels[i]] < 5) {
                std::cout << "Validation sample " << i << ": outputs["
                        << outputMatrix.getValue(0, 0) << ", "
                        << outputMatrix.getValue(1, 0) << ", "
                        << outputMatrix.getValue(2, 0) << ", "
                        << outputMatrix.getValue(3, 0) << "] -> predicted="
                        << predicted << ", actual=" << labels[i] << std::endl;
                printCount[labels[i]]++;
            }
        }
    else if (rows == 1 && cols == 4) {
        maxVal = outputMatrix.getValue(0, 0);
        for (int k = 1; k < 4; ++k) {
            double val = outputMatrix.getValue(0, k);
            if (val > maxVal) {
                maxVal = val;
                predicted = k;
            }
        }
        if (printCount[labels[i]] < 5) {
            std::cout << "Validation sample " << i << ": outputs["
                    << outputMatrix.getValue(0, 0) << ", "
                    << outputMatrix.getValue(0, 1) << ", "
                    << outputMatrix.getValue(0, 2) << ", "
                    << outputMatrix.getValue(0, 3) << "] -> predicted="
                    << predicted << ", actual=" << labels[i] << std::endl;
            printCount[labels[i]]++;
        }
    }

        else {
            // Unexpected matrix dimension
            std::cout << "Unexpected output matrix dimensions during validation: "
                      << rows << "x" << cols << std::endl;
            continue;   // Skipping this sample
        }

        // Lets check if the prediction matches the actual label
        if (i < labels.size() && predicted == labels[i]) {
            correct++;
        }

    }

    // Calculating and returning validation accuracy
    if (features.empty()) {
        std::cout << "Warning: Empty validation dataset!" << std:: endl;
    }

    double accuracy = (double)correct / features.size();
    std::cout << "Validation Accuracy: " << correct << "/" << features.size()
                << " = " << accuracy * 100 << "%" << std::endl;
                
    return accuracy;
}

}   // Namespace NeuroRF