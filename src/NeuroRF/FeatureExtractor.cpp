#include "..\..\include\NeuroRF\FeatureExtractor.hpp"

#include <vector>
#include <cmath>
#include <numeric>
#include <complex>

double NeuroRF::FeatureExtractor::calculateMean(const std::vector<double> &data) {
    int n = data.size();
    if ( n == 0 ) return 0.0;

    auto first  = data.begin();
    auto last   = data.end();

    double sum  = std::accumulate(first, last, 0.0);

    return (sum / n);
}

double NeuroRF::FeatureExtractor::calculateVariance(const std::vector<double> &data, bool isSample) {
    int n = data.size();
    if (n == 0 || (isSample && n == 1)) return 0.0;     // To avoid zero division crash

    double mean = calculateMean(data);

    double sumDiffSquared = 0.00;

    for (double item : data) {
        double sumDiff = mean - item;
        sumDiffSquared += sumDiff * sumDiff;
    }

    if (isSample) return (sumDiffSquared / (n - 1));
    return (sumDiffSquared / n);    // else
}

double NeuroRF::FeatureExtractor::calculateStdDev(const std::vector<double> &data, bool isSample) {
    double variance = calculateVariance(data, isSample);
    double stdDev = sqrt(variance);
    return stdDev;
}
