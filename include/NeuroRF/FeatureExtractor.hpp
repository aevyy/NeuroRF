#pragma once

#include <cmath> 
#include <numeric>
#include <vector>
#include <complex>

namespace NeuroRF {

class FeatureExtractor {
public:

    // Main interface
    std::vector<double> basicFeatures(const std::vector<std::complex<double>> sample);

    double getMean(const std::vector<double> &data);
    double getVariance(const std::vector<double> &data, bool isSample);
    double getStdDev(const std::vector<double> &data, bool isSample);

    std::vector<double> getIComponents(const std::vector<std::complex<double>> data);
    std::vector<double> getQComponents(const std::vector<std::complex<double>> data);

private:

};
}