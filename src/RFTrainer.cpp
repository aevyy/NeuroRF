#include "../include/RFTrainer.hpp"
#include "../include/DataLoader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

namespace NeuroRF {

RFTrainer::RFTrainer() : network({6, 12, 8, 2}) {}

void RFTrainer::train(const std::string& csvFile, int epochs) {
    std::vector<std::vector<double>> features;
    std::vector<int> labels;
    DataLoader::loadData(csvFile, features, labels);
    
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
            network.setCurrentTarget(one_hot);
            network.backPropagation();
        }
        if (epoch % 10 == 0) {
            std::cout << "Epoch " << epoch << "/" << epochs << std::endl;
        }
    }
}

double RFTrainer::test(const std::string& csvFile) {
    std::vector<std::vector<double>> features;
    std::vector<int> labels;
    DataLoader::loadData(csvFile, features, labels);
    
    int correct = 0;
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
        } 
        else if (rows == 1 && cols == 2) {
            // 1x2 matrix (row vector)
            double output0 = output_matrix.getValue(0, 0);
            double output1 = output_matrix.getValue(0, 1);
            predicted = (output0 > output1) ? 0 : 1;
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

}