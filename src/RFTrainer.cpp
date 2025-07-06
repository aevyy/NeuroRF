#include "../include/RFTrainer.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <numeric>

namespace NeuroRF {

RFTrainer::RFTrainer() : network({6, 12, 8, 2}) {}

void RFTrainer::train(const std::string& csvFile, int epochs) {
    std::ifstream file(csvFile);
    std::vector<std::vector<double>> features;
    std::vector<std::vector<double>> labels;
    
    std::string line;
    std::getline(file, line); // skip header
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<double> feature_row;
        
        for (int i = 0; i < 6; i++) {
            std::getline(ss, cell, ',');
            feature_row.push_back(std::stod(cell));
        }
        
        std::getline(ss, cell, ',');
        int label = std::stoi(cell);
        std::vector<double> label_row = {0.0, 0.0};
        label_row[label] = 1.0;
        
        features.push_back(feature_row);
        labels.push_back(label_row);
    }
    
    std::cout << "Loaded " << features.size() << " training samples" << std::endl;
    std::cout << "Training with learning rate: 0.1, bias: 1.0, momentum: 1.0" << std::endl;
    
    // SHUFFLE the data before training
    std::vector<size_t> indices(features.size());
    std::iota(indices.begin(), indices.end(), 0);
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);
    
    std::cout << "Data shuffled for training" << std::endl;
    
    for (int epoch = 0; epoch < epochs; epoch++) {
        std::shuffle(indices.begin(), indices.end(), g);  // Every epoch!

        for (size_t idx = 0; idx < indices.size(); idx++) {
            size_t i = indices[idx];  // Use shuffled index
            
            network.train(features[i], labels[i], 1.0, 0.1, 1.0);  // bias=1.0, learningRate=0.1, momentum=1.0
        }
        
        if (epoch % 10 == 0) {
            std::cout << "Epoch " << epoch << "/" << epochs << std::endl;
        }
    }
}

double RFTrainer::test(const std::string& csvFile) {
    std::ifstream file(csvFile);
    std::vector<std::vector<double>> features;
    std::vector<int> labels;
    
    std::string line;
    std::getline(file, line); // skip header
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<double> feature_row;
        
        for (int i = 0; i < 6; i++) {
            std::getline(ss, cell, ',');
            feature_row.push_back(std::stod(cell));
        }
        
        std::getline(ss, cell, ',');
        labels.push_back(std::stoi(cell));
        features.push_back(feature_row);
    }
    
    std::cout << "Testing on " << features.size() << " samples" << std::endl;
    
    int correct = 0;
    int bpsk_correct = 0, bpsk_total = 0;
    int qpsk_correct = 0, qpsk_total = 0;
    
    for (size_t i = 0; i < features.size(); i++) {
        network.setCurrentInput(features[i]);
        network.feedForward();

        Matrix output_matrix = network.getActivatedNeuronMatrix(network.topologySize - 1);
        double output0 = output_matrix.getValue(0, 0);
        double output1 = output_matrix.getValue(0, 1);
        
        int predicted = (output0 > output1) ? 0 : 1;

        // DEBUG: Printing detailed info for few first and last samples
        if (i < 4 || i >= features.size() - 4) {
            std::cout << "Sample " << i + 1 << " (label=" << labels[i] << "):" << std::endl;
            std::cout << "  Features: ";
            for (int f = 0; f < 6; f++) std::cout << features[i][f] << " ";
            std::cout << std::endl;
            std::cout << "  Outputs: [" << output0 << ", " << output1 << "] -> predicted=" << predicted << std::endl;
            std::cout << "  " << (predicted == labels[i] ? "CORRECT" : "WRONG") << std::endl;
        }

        if (predicted == labels[i]) correct++;

        // Track performance by class
        if (labels[i] == 0) {
            bpsk_total++;
            if (predicted == 0) bpsk_correct++;
        } else {
            qpsk_total++;
            if (predicted == 1) qpsk_correct++;
        }
    }
    
    std::cout << "Performance:" << std::endl;
    std::cout << "  BPSK: " << bpsk_correct << "/" << bpsk_total << " = " 
              << (100.0 * bpsk_correct / bpsk_total) << "%" << std::endl;
    std::cout << "  QPSK: " << qpsk_correct << "/" << qpsk_total << " = " 
              << (100.0 * qpsk_correct / qpsk_total) << "%" << std::endl;

    return (double)correct / features.size();
}

}   // Namespace NeuroRF