#pragma once

#include "../CustomNN/headers/NeuralNetwork.h"

#include <vector>
#include <string>

namespace NeuroRF {
    class RFTrainer {
    private:
        NeuralNetwork network;

    public:
        RFTrainer();
        void train(const std::string &csvFile, int epochs);
        double test(const std::string &csvFile);
        double validate(const std::string &csvFile);
        NeuralNetwork& getNetwork() { return network; }
    };
}