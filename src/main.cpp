#include "../include/NeuroRF/SignalGenerator.hpp"
#include <iostream>

int main() {
    // Test signal generation
    NeuroRF::SignalGenerator generate;
    std::cout << "BPSK bit 0 generation: " << generate.generateBPSK(0) << std::endl;
    std::cout << "BPSK bit 1 generation: " << generate.generateBPSK(1) << std::endl;
    std::cout << "QPSK bit 0, 0 generation: " << generate.generateQPSK(0,0) << std::endl;
    std::cout << "QPSK bit 0, 1 generation: " << generate.generateQPSK(0,1) << std::endl;
    std::cout << "QPSK bit 1, 0 generation: " << generate.generateQPSK(1,0) << std::endl;
    std::cout << "QPSK bit 1, 1 generation: " << generate.generateQPSK(1,1) << std::endl;
}