
#include "../include/NeuroRF/SignalGenerator.hpp"
#include "../include/RFTrainer.hpp"
#include "../include/TestFunctions.hpp"
#include <iostream>
#include <filesystem>

int main() {
    NeuroRF::SignalGenerator generator;
    // Testing 8PSK generation
    std::vector<int> bits = {0, 0, 0, 1, 1, 0, 0, 0, 0};
    auto foo = generator.generate8PSKSequence(bits);

    std::cout << foo[1] << foo[2] << foo[0] << std::endl;


    return 0;
}