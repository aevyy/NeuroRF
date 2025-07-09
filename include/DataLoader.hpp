#pragma once

#include <vector>
#include <string>

class DataLoader {
public:
    void loadData(const std::string &fileName);

    // Getters
    const std::vector<std::vector<double>> getFeatures() { return this->features; }
    const std::vector<int> getLabels() { return this->labels; }

    // Utils
    size_t size() const { return this->features.size(); }
    int clear() const {return features.empty(); }

    // To clear the loaded data
    void clear() { features.clear(); labels.clear(); }

    
private:
    std::vector<std::vector<double>> features;
    std::vector<int> labels;
};