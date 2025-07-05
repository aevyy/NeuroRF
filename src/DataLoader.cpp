#include "../include/DataLoader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

void DataLoader::loadTrainingData(
    const std::string &fileName,
    std::vector<std::vector<double>> &features,
    std::vector<std::vector<double>> &labels) {

    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + fileName);
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<double> featureRow;

        // Lets read the current 6 features that we have
        for (int i = 0; i < 6; i++) {
            std::getline(ss, cell, ',');
            featureRow.push_back(std::stod(cell));
        }

        // Now the labels
        std::getline(ss, cell, ',');
        int label = std::stoi(cell);

        // Lets convert the labels to one hot encoding
        std::vector<double> labelRow = {0.0, 0.0};
        labelRow[label] = 1.0;      // (1, 0) for BPSK & (0, 1) for QPSK

        features.push_back(featureRow);
        labels.push_back(labelRow);
    }

    std::cout << "Loaded " << features.size() << " training samples" << std::endl;    
}