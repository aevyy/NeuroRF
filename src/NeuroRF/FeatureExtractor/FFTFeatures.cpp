#include "..\..\..\include\NeuroRF\FeatureExtractor.hpp"

std::vector<double> NeuroRF::FeatureExtractor::FFTFeatures(const std::vector<std::complex<double>> &sample) {
    dsp::fft FFTransform;
    std::vector<std::complex<double>> spectrum = FFTransform.compute(sample);

    // Lets extract the magnitudes
    std::vector<double> magnitudes;
    for (const auto& bin : spectrum) {
        magnitudes.push_back(std::abs(bin));
    }

    // Now, lets get the spectral features
    std::vector<double> features;
    for (int i = 0; i < 5 && i < magnitudes.size(); i++) {
        features.push_back(magnitudes[i]);
    }

    // Maximum FFT magnitude (strongest frequency component)
    double maxMag = *std::max_element(magnitudes.begin(), magnitudes.end());
    features.push_back(maxMag);

    // Maximum FFT bin (index of strongest frequency component)
    auto maxIt = std::max_element(magnitudes.begin(), magnitudes.end());
    double maxBin = static_cast<double>(std::distance(magnitudes.begin(), maxIt));
    features.push_back(maxBin);

    // Spectral centroid (center of mass of spectrun)
    double weightedSum = 0.0;
    double magSum = std::accumulate(magnitudes.begin(), magnitudes.end(), 0.0);
    for (size_t i = 0; i < magnitudes.size(); i++) {
        weightedSum += i * magnitudes[i];
    }
    double spectralCentroid = (magSum > 0.0) ? (weightedSum / magSum) : 0.0;
    features.push_back(spectralCentroid);

    // Spectral variance (spread of spectrum)
    double varianceSum = 0.0;
    for (size_t i = 0; i < magnitudes.size(); i++) {
        varianceSum += magnitudes[i] * std::pow(i - spectralCentroid, 2);
    }
    double spectralVariance = (magSum > 0.0) ? (varianceSum / magSum) : 0.0;
    features.push_back(spectralVariance);

    return features;
}