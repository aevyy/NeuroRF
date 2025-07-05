#pragma once

#include <vector>
#include <string>

class DataLoader {
public:
    static void loadData(
        const std::string &fileName,
        std::vector<std::vector<double>> &features,
        std::vector<int> &labels
    );
};