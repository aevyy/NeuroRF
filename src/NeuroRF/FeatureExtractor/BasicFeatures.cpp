#include "..\..\..\include\NeuroRF\FeatureExtractor.hpp"

#include <vector>
#include <cmath>
#include <numeric>
#include <complex>

std::vector<double> NeuroRF::FeatureExtractor::basicFeatures(const std::vector<std::complex<double>> &sample) {
    std::vector<double> I_values = getIComponents(sample);
    std::vector<double> Q_values = getQComponents(sample);

    double I_values_mean        = getMean(I_values);
    double I_values_variance    = getVariance(I_values, false);
    double I_values_stdDev      = getStdDev(I_values, false);

    double Q_values_mean        = getMean(Q_values);
    double Q_values_variance    = getVariance(Q_values, false);
    double Q_values_stdDev      = getStdDev(Q_values, false);

    std::vector<double> features;
    // Basic I/Q features
    features.push_back(I_values_mean);
    features.push_back(I_values_variance);
    features.push_back(I_values_stdDev);
    features.push_back(Q_values_mean);
    features.push_back(Q_values_variance);
    features.push_back(Q_values_stdDev);

    // --- FFT-based spectral features ---
    std::vector<double> fft_features = FFTFeatures(sample);
    features.insert(features.end(), fft_features.begin(), fft_features.end());

    return features;
}

std::vector<double> NeuroRF::FeatureExtractor::getIComponents(const std::vector<std::complex<double>> data) {
    std::vector<double> i_values;

    for (std::complex<double> IQs : data) {
        i_values.push_back(IQs.real());
    }

    return i_values;
}

std::vector<double> NeuroRF::FeatureExtractor::getQComponents(const std::vector<std::complex<double>> data) {
    std::vector<double> q_values;

    for (std::complex<double> IQs : data) {
        q_values.push_back(IQs.imag());
    }

    return q_values;
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
