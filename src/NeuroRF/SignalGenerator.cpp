#include "..\..\include\NeuroRF\SignalGenerator.hpp"
#include "..\..\include\NeuroRF\FeatureExtractor.hpp"

// Constructor: initializes the class with random generator & random seed
NeuroRF::SignalGenerator::SignalGenerator() : generator(rd()) {}


std::complex<double> NeuroRF::SignalGenerator::generateBPSK(int bit) {
    if (bit != 0 && bit != 1) {
        throw std::invalid_argument("Bit must be either 0 or 1");
    }

    if (bit == 0) return std::complex<double>(+1.0, 0.0);
    else return std::complex<double>(-1.0, 0.0);
}


std::complex<double> NeuroRF::SignalGenerator::generateQPSK(int bit1, int bit2) {
    if ((bit1 != 0 && bit1 != 1) || (bit2 != 0 && bit2 != 1)) {
        throw std::invalid_argument("Bits must be either 0 or 1");
    }
    
    // Normalizing by sqrt(2) to have same power as BPSK
    double scale = 1.0 / std::sqrt(2.0);
    
    if (bit1 == 0 && bit2 == 0) {
        return std::complex<double>(+1.0 * scale, +1.0 * scale);
    }
    else if (bit1 == 0 && bit2 == 1) {
        return std::complex<double>(-1.0 * scale, +1.0 * scale);
    }
    else if (bit1 == 1 && bit2 == 0) {
        return std::complex<double>(-1.0 * scale, -1.0 * scale);
    }
    else {
        return std::complex<double>(+1.0 * scale, -1.0 * scale);
    }
}


/* 
Ref: https://en.wikipedia.org/wiki/Independent_and_identically_distributed_random_variables

Principle:
'''
In probability theory and statistics, a collection of random variables
is independent and identically distributed (i.i.d., iid, or IID) if each
random variable has the same probability distribution as the others and
all are mutually independent. IID was first defined in statistics and
finds application in many fields, such as data mining and signal processing.
'''
*/
std::vector<std::complex<double>> NeuroRF::SignalGenerator::addNoise(
    const std::vector<std::complex<double>> &signal,
    double noisePeak) {
        std::normal_distribution<double> noise(0.0, noisePeak);
        std::vector<std::complex<double>> noisySignal;

        for (const auto &sample : signal) {
            double i_noise = noise(this->generator);
            double q_noise = noise(this->generator);

            noisySignal.push_back(sample + std::complex<double> (i_noise, q_noise));
        }

        return noisySignal;
    }


std::vector<std::complex<double>> NeuroRF::SignalGenerator::generateBPSKSequence(const std::vector<int> &bits) {
    std::vector<std::complex<double>> signals;

    for (const auto &bit : bits) {
        signals.push_back(this->generateBPSK(bit));
    }

    return signals;
}

std::vector<std::complex<double>> NeuroRF::SignalGenerator::generateQPSKSequence(const std::vector<std::pair<int,int>> &bits) {
    std::vector<std::complex<double>> signals;

    for (const auto &bitPair : bits) {
        signals.push_back(generateQPSK(bitPair.first, bitPair.second));
    }

    return signals;
}


void NeuroRF::SignalGenerator::generateTrainingCSV(int samplesPerClass) {
    // 60% train, 20% test, 20% validation
    int trainSamples = static_cast<int>(samplesPerClass * 0.6);
    int testSamples = static_cast<int>(samplesPerClass * 0.2);
    int valiSamples = static_cast<int>(samplesPerClass - trainSamples - testSamples);

    std::vector<std::ofstream> files;
    files.emplace_back(std::ofstream("trainingData.csv"));
    files.emplace_back(std::ofstream("testingData.csv"));
    files.emplace_back(std::ofstream("validationData.csv"));

    std::vector<std::string> fileNames = {
        "trainingData.csv",
        "testingData.csv",
        "validationData.csv"
    };

    for (int i = 0; i < files.size(); i++) {
        if (!files[i].is_open()) {
            throw std::runtime_error("Cannot open " + fileNames[i]);
        }
    }

    for (int i = 0; i < files.size();  i++) {
        files[i] << "I_mean,I_variance,I_stdDev,Q_mean,Q_variance,Q_stdDev,label\n";
    }

    FeatureExtractor extractor;

    for (int label = 0; label < 2; label++) {
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
                else {
                    std::vector<std::pair<int, int>> bits(16);
                    for (auto &bitPair : bits) bitPair = {this->generator() % 2, this->generator() % 2};
                    signal = this->generateQPSKSequence(bits);
                }

                std::vector<std::complex<double>> noisySignal = this->addNoise(signal, 0.1);
                std::vector<double> features = extractor.basicFeatures(noisySignal);

                // Lets write to appropriate files
                for (size_t j = 0; j < features.size(); j++) {
                    files[fileIdx] << features[j] << ",";
                }
                files[fileIdx] << label << "\n";
            }
        }

        std::cout << (label == 0 ? "BPSK" : "QPSK") << "signals generated for all datasets.";
    }

    std::cout << "Generated " << trainSamples << " training, " << testSamples
        << " testing, " << valiSamples << " validation samples per class.\n";
}