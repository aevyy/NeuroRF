#include "..\..\..\include\NeuroRF\SignalGenerator.hpp"

std::complex<double> NeuroRF::SignalGenerator::generate16QAM(int bit1, int bit2, int bit3, int bit4) {
    if ((bit1 != 0 && bit1 != 1) || (bit2 != 0 && bit2 != 1) ||
        (bit3 != 0 && bit3 != 1) || (bit4 != 0 && bit4 != 1)) {
            throw std::invalid_argument("Bits must be either 0 or 1");
    }

    // Lets convert the bits' combination to decimal (0-15)
    int dec = (bit1 << 3) | (bit2 << 2) | (bit3 << 1) | bit4;

    // Now, extracting I and Q components with bit manipulation
    // For Gray coding: first 2 bits determine I, and last 2 determine Q
    int i_bits = (bit1 << 1) | bit2;    // Firt 2 bits for I component
    int q_bits = (bit3 << 1) | bit4;    // Last 2 bits for Q component

    // Lets map 2-bit valies to constellation levels:
    // 00->-3, 01->-1, 11->+1, 10->+3
    double i_levels[] = {-3.0, -1.0, +3.0, +1.0};   // Grey coded: 00, 01, 11, 10
    double q_levels[] = {-3.0, -1.0, +3.0, +1.0};

    double i_component = i_levels[i_bits];
    double q_component = q_levels[q_bits];

    // Normalization factor for unit average power
    double scale = 1.0 / std::sqrt(10.0);

    return std::complex<double>(i_component * scale, q_component * scale);
}

std::vector<std::complex<double>> NeuroRF::SignalGenerator::generate16QAMSequence(const std::vector<int> &bits) {
    std::vector<std::complex<double>> signals;

    for (int i = 0; i < bits.size() - 3; i += 4) {
        signals.push_back(generate16QAM(bits[i], bits[i+1], bits[i+2], bits[i+3]));
    }

    return signals;
}