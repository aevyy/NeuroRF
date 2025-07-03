#pragma once

#include <cmath> 
#include <numeric>
#include <vector>
#include <complex>

namespace NeuroRF {

class FeatureExtractor {
public:
    double calculateMean(const std::vector<double> &data);
    double calculateVariance(const std::vector<double> &data);
    double calculateStdDev(const std::vector<double> &data);
    
private:

};
}