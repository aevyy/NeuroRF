#include "../include/DataLoader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

// Can be used for both training and testing
void DataLoader::loadData(
    const std::string &fileName,
    std::vector<std::vector<double>> &features,
    std::vector<int> &labels) {

    std::ifstream file(fileName);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + fileName);
    }

    std::string line;
    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<double> featureRow;
        bool malformed = false;

        // Read 6 features
        for (int i = 0; i < 6; i++) {
            if (!std::getline(ss, cell, ',')) { malformed = true; break; }
            try {
                featureRow.push_back(std::stod(cell));
            } catch (...) { malformed = true; break; }
        }
        if (malformed) continue;
        
        if (!std::getline(ss, cell, ',')) continue;
        int label;
        try {
            label = std::stoi(cell);
        } catch (...) { continue; }

        if (label < 0 || label > 1) continue; // Valid labels for now: 0 or 1

        features.push_back(featureRow);
        labels.push_back(label);
    }

    if (features.size() != labels.size() || features.empty()) {
        throw std::runtime_error("Mismatch or empty features/labels in data!");
    }

    std::cout << "Loaded " << features.size() << " samples" << std::endl;    
}