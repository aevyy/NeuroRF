
#include "../include/NeuroRF/SignalGenerator.hpp"
#include "../include/RFTrainer.hpp"
#include "../include/TestFunctions.hpp"
#include <iostream>
#include <filesystem>

int main() {
    NeuroRF::SignalGenerator generator;
    // Testing 8PSK generation
    auto mod8PSKData1 = generator.generate8PSK(1, 0, 1);
    auto mod8PSKData2 = generator.generate8PSK(1, 1, 1);
    auto mod8PSKData3 = generator.generate8PSK(0, 0, 1);
    auto mod8PSKData4 = generator.generate8PSK(0, 0, 0);

    std::cout << mod8PSKData1 << std::endl;
    std::cout << mod8PSKData2 << std::endl;
    std::cout << mod8PSKData3 << std::endl;
    std::cout << mod8PSKData4 << std::endl;

    return 0;
}