#include "..\..\include\NeuroRF\SignalGenerator.hpp"
#include "..\..\include\NeuroRF\FeatureExtractor.hpp"

// For some reason, I'm getting errors trying to use M_PI from standard headers
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Constructor: initializes the class with random generator & random seed
NeuroRF::SignalGenerator::SignalGenerator() : generator(rd()) {}

std::complex<double> NeuroRF::SignalGenerator::generate8PSK(int bit1, int bit2, int bit3) {
    /*
    My strategy to generate the respective signals as per  the bits
    combinations is to first convert the bits combo to its respective decimal
    form, i.e. 0-7 (binary 111, which is the max we can get as argument,
    gives 7). Then it gets easier to get the respective signals using an
    expression:  angle = 2 * pi * k/8   
    */

    if ((bit1 != 0 && bit1 != 1) || (bit2 != 0 && bit2 != 1) || (bit3 != 0 && bit3 != 1)) {
        throw std::invalid_argument("Bits must be either 1 or 0");
    }

    // First, lets convert the bits' combo to decimals (0-7)
    int dec = (bit1 << 2) | (bit2 << 1) | (bit3);    // == (bit1 * 4) + (bit2 * 2) + bit3
    double angle = 2.0 * M_PI * dec / 8.0;
    return std::polar(1.0, angle); // Gives the complex number for radius 1 and the given angle
}

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

std::vector<std::complex<double>> NeuroRF::SignalGenerator::generate8PSKSequence(const std::vector<int> &bits) {
    std::vector<std::complex<double>> signals;
    NeuroRF::SignalGenerator generator;
    for (int i = 0; i < bits.size() - 2; i += 3) {
        signals.push_back(generator.generate8PSK(bits[i], bits[i+1], bits[i+2]));
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

    for (int label = 0; label < 3; label++) {
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
                    std::vector<int> bits(18);
                    for (auto &bitTrio : bits) bitTrio = static_cast<int>(this->generator() % 2);
                    signal = this->generate8PSKSequence(bits);
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