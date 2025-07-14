#include "..\..\..\include\NeuroRF\SignalGenerator.hpp"
#include "..\..\..\include\NeuroRF\FeatureExtractor.hpp"

void NeuroRF::SignalGenerator::generateTrainingCSV(int samplesPerClass) {
    // 60% train, 20% test, 20% validation
    int trainSamples = static_cast<int>(samplesPerClass * 0.6);
    int testSamples = static_cast<int>(samplesPerClass * 0.2);
    int valiSamples = static_cast<int>(samplesPerClass - trainSamples - testSamples);

    std::vector<std::ofstream> files;
    files.emplace_back(std::ofstream("../trainingData.csv"));
    files.emplace_back(std::ofstream("../testingData.csv"));
    files.emplace_back(std::ofstream("../validationData.csv"));

    std::vector<std::string> fileNames = {
        "../trainingData.csv",
        "../testingData.csv",
        "../validationData.csv"
    };

    for (int i = 0; i < files.size(); i++) {
        if (!files[i].is_open()) {
            throw std::runtime_error("Cannot open " + fileNames[i]);
        }
    }

    for (int i = 0; i < files.size();  i++) {
        files[i] << "I_mean,I_variance,I_stdDev,Q_mean,Q_variance,Q_stdDev";
        for (int k = 0; k < 5; k++) files[i] << ",FFT_mag" << k;
        files[i] << ",FFT_maxMag,FFT_maxBin,FFT_centroid,FFT_variance,label\n";
    }

    FeatureExtractor extractor;

    for (int label = 0; label < 4; label++) {
        std::vector<int> sampleCounts = {trainSamples, testSamples, valiSamples};

        for (int fileIdx = 0; fileIdx < 3; fileIdx++) {
            for (int i = 0; i < sampleCounts[fileIdx]; i++) {
                std::vector<std::complex<double>> signal;

                // BPSK
                if (label == 0) {
                    std::vector<int> bits(16);
                    for (int &bit : bits) bit = this->generator() % 2;
                    signal = this->generateBPSKSequence(bits);
                }

                // QPSK
                else if (label == 1) {
                    std::vector<std::pair<int, int>> bits(16);
                    for (auto &bitPair : bits) bitPair = {this->generator() % 2, this->generator() % 2};
                    signal = this->generateQPSKSequence(bits);
                }

                // 8PSK
                else if (label == 2) {
                    std::vector<int> bits(48);  // Generate 48/3 = 16 signals
                    for (auto &bitTrio : bits) bitTrio = static_cast<int>(this->generator() % 2);
                    signal = this->generate8PSKSequence(bits);
                }

                else if (label == 3) {
                    std::vector<int> bits(64);
                    for (auto& bitQuad : bits) bitQuad = static_cast<int>(this->generator() % 2);
                    signal = this->generate16QAMSequence(bits);
                }

                std::vector<std::complex<double>> noisySignal = this->addNoise(signal, 0.05);
                std::vector<double> features = extractor.basicFeatures(noisySignal);

                // Lets write to appropriate files
                for (size_t j = 0; j < features.size(); j++) {
                    files[fileIdx] << features[j] << ",";
                }
                files[fileIdx] << label << "\n";
            }
        }

        std::cout << (label == 0 ? "BPSK" : label == 1 ? "QPSK" : label == 2 ? "8PSK" : label == 3 ? "16QAM" : "Undetected") << "signals generated for all datasets.\n";
    }

    std::cout << "Generated " << trainSamples << " training, " << testSamples
        << " testing, " << valiSamples << " validation samples per class.\n";
}