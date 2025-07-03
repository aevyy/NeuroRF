#include "..\..\include\NeuroRF\FeatureExtractor.hpp"

#include <vector>
#include <cmath>
#include <numeric>
#include <complex>

std::vector<double> NeuroRF::FeatureExtractor::basicFeatures(const std::vector<std::complex<double>> sample) {
    std::vector<double> features;
    std::vector<double> I_values;
    std::vector<double> Q_values;

    for (std::complex<double> data : sample) {
        I_values.push_back(data.real());
        Q_values.push_back(data.imag());
    }

    double I_values_mean        = getMean(I_values);
    double I_values_variance    = getVariance(I_values, false);
    double I_values_stdDev      = getStdDev(I_values, false);

    double Q_values_mean        = getMean(Q_values);
    double Q_values_variance    = getVariance(Q_values, false);
    double Q_values_stdDev      = getStdDev(Q_values, false);

    features.push_back(I_values_mean);
    features.push_back(I_values_variance);
    features.push_back(I_values_stdDev);
    features.push_back(Q_values_variance);
    features.push_back(Q_values_mean);
    features.push_back(Q_values_stdDev);

    return features;
}

double NeuroRF::FeatureExtractor::getMean(const std::vector<double> &data) {
    int n = data.size();
    if ( n == 0 ) return 0.0;

    auto first  = data.begin();
    auto last   = data.end();

    double sum  = std::accumulate(first, last, 0.0);

    return (sum / n);
}

double NeuroRF::FeatureExtractor::getVariance(const std::vector<double> &data, bool isSample) {
    int n = data.size();
    if (n == 0 || (isSample && n == 1)) return 0.0;     // To avoid zero division crash

    double mean = getMean(data);

    double sumDiffSquared = 0.00;

    for (double item : data) {
        double sumDiff = mean - item;
        sumDiffSquared += sumDiff * sumDiff;
    }

    if (isSample) return (sumDiffSquared / (n - 1));
    return (sumDiffSquared / n);    // else
}

double NeuroRF::FeatureExtractor::getStdDev(const std::vector<double> &data, bool isSample) {
    double variance = getVariance(data, isSample);
    double stdDev   = sqrt(variance);
    return stdDev;
}
