#pragma once

#include <vector>
#include <string>

class DataLoader {
public:
    static void loadTrainingData(
        const std::string &fileName,
        std::vector<std::vector<double>> &features,
        std::vector<std::vector<double>> &labels
    );
};