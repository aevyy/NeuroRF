#include "../include/RFTrainer.hpp"
#include "../include/DataLoader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

namespace NeuroRF {

RFTrainer::RFTrainer() : network({6, 12, 8, 2}) {}

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
            std::vector<double> one_hot(2, 0.0);
            if (labels[i] >= 0 && labels[i] < 2) {
                one_hot[labels[i]] = 1.0;
            }
            
            // DEBUG: Print first few training examples from both classes
            if (epoch == 0 && (i < 5 || (i >= features.size()/2 && i < features.size()/2 + 5))) {
                std::cout << "Training sample " << i << ": label=" << labels[i] 
                          << " -> one_hot=[" << one_hot[0] << ", " << one_hot[1] << "]" << std::endl;
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
    std::cout << "here" << std::endl;
    std::cout << features.size();

    network.setCurrentTarget(features[features.size() - 1]);

    for (size_t i = 0; i < features.size(); i++) {
        network.setCurrentInput(features[i]);
        network.feedForward();
        
        // output from last layer
        Matrix output_matrix = network.getActivatedNeuronMatrix(network.topologySize - 1);
        
        // Checking matrix dimensions before accessing
        int rows = output_matrix.getNumRows();
        int cols = output_matrix.getNumCols();
        
        // Determining prediction based on matrix shape
        int predicted;
        if (rows == 2 && cols == 1) {
            // 2x1 matrix (column vector)
            double output0 = output_matrix.getValue(0, 0);
            double output1 = output_matrix.getValue(1, 0);
            predicted = (output0 > output1) ? 0 : 1;
            
            // DEBUG: Print first 10 and last 10 predictions
            if (i < 10 || i >= features.size() - 10) {
                std::cout << "Sample " << i << ": outputs[" << output0 << ", " << output1 
                          << "] -> predicted=" << predicted << ", actual=" << labels[i] << std::endl;
            }
        } 
        else if (rows == 1 && cols == 2) {
            // 1x2 matrix (row vector)
            double output0 = output_matrix.getValue(0, 0);
            double output1 = output_matrix.getValue(0, 1);
            predicted = (output0 > output1) ? 0 : 1;
            
            // DEBUG: Print first 10 and last 10 predictions
            if (i < 10 || i >= features.size() - 10) {
                std::cout << "Sample " << i << ": outputs[" << output0 << ", " << output1 
                          << "] -> predicted=" << predicted << ", actual=" << labels[i] << std::endl;
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


        int predicted;

        if (rows == 2 && cols == 1) {
            // 2 x 1 matrix (column vector output)
            double output0  = outputMatrix.getValue(0, 0);   // Output for class 0 (BPSK)
            double output1  = outputMatrix.getValue(1, 0);   // Output for class 1 (QPSK)
            predicted       = ( output0 > output1) ? 0 : 1;


            // Debug: Printing first 5 and last 5 validation predictions
            if (i < 5 || i >= features.size() - 5) {
                std::cout << "Validation sample " << i << ": outputs[" << output0 << ", " << output1 
                          << "] -> predicted=" << predicted << ", actual=" << labels[i] << std::endl;
            }
        }

        else if (rows == 1 && cols == 2) {
            // 1 x 2 matrix (row vector output)
            double output0  = outputMatrix.getValue(0, 0);
            double output1  = outputMatrix.getValue(0, 1);
            predicted       = ( output0 > output1 ) ? 0 : 1;

            // Debug: Printing first 5 and last 5 validation predictions
            if (i < 5 || i >= features.size() - 5) {
                std::cout << "Validation sample " << i << ": outputs[" << output0 << ", " << output1 
                          << "] -> predicted=" << predicted << ", actual=" << labels[i] << std::endl;
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