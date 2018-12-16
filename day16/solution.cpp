// g++ -std=c++17 -O3 -Wall -c solution.cpp -o solution.o

#include "cpu.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <tuple>
#include <optional>
#include <sstream>
#include <fstream>
#include <iostream>

#include <cassert>

struct Sample
{
    Cpu cpu_before;
    Instruction instruction;
    Cpu cpu_after;
};

std::ostream& operator<<(std::ostream& os, const Sample& sample)
{
    os << "=============================\n";
    os << "Before: ";
    for (auto reg : sample.cpu_before.gpr) {
        os << static_cast<int>(reg) << " ";
    }
    os << '\n';
    os << "Instr: "
       << static_cast<int>(sample.instruction.opcode) << " "
       << static_cast<int>(sample.instruction.a_in) << " "
       << static_cast<int>(sample.instruction.b_in) << " "
       << static_cast<int>(sample.instruction.c_out) << '\n';
    os << "After: ";
    for (auto reg : sample.cpu_after.gpr) {
        os << static_cast<int>(reg) << " ";
    }
    os << '\n';
    return os;
}

uint8_t char_to_int(char c)
{
    assert(c >= '0' && c <= '9');
    return c - '0';
}

// Before: [1, 1, 3, 0]
// 12 3 2 0
// After:  [1, 1, 3, 0]
Cpu read_cpu_state(std::string line)
{
    return Cpu{
        { char_to_int(line[9]),
          char_to_int(line[12]),
          char_to_int(line[15]),
          char_to_int(line[18]) },
        0
    };
}

Instruction read_instruction_line(std::string line)
{
    std::stringstream ss(line);
    int opcode = 0;
    int a_in = 0;
    int b_in = 0;
    int c_out = 0;
    ss >> opcode;
    ss >> a_in;
    ss >> b_in;
    ss >> c_out;
    return Instruction{static_cast<uint8_t>(opcode),
                       static_cast<uint8_t>(a_in),
                       static_cast<uint8_t>(b_in),
                       static_cast<uint8_t>(c_out)};
}

std::optional<Sample> read_sample(std::ifstream& filein)
{
    std::string beforeline;
    if (!std::getline(filein, beforeline) || beforeline.rfind("Before", 0) != 0)
        return {};

    std::string instructionline;
    if (!std::getline(filein, instructionline))
        return {};

    std::string afterline;
    if (!std::getline(filein, afterline) || afterline.rfind("After", 0) != 0)
        return {};

    return Sample{
        read_cpu_state(beforeline),
        read_instruction_line(instructionline),
        read_cpu_state(afterline)
    };
}

int count_matches(Sample& sample,
                  const std::map<std::string, InstructionFunc*>& instructions)
{
    // test and remove opcodes that produce different results than expected
    return std::count_if(std::begin(instructions), std::end(instructions),
                       [&sample](const auto& it) {
                            Cpu expected{sample.cpu_before};
                            auto handler = *it.second;
                            handler(expected,
                                    sample.instruction.a_in,
                                    sample.instruction.b_in,
                                    sample.instruction.c_out);
                            return sample.cpu_after == expected;
                       });
}

int main()
{
    std::map<uint8_t, std::vector<std::string>> matches;
    std::ifstream data("data.txt");
    int total_samples = 0;
    int over_three = 0;
    while (data) {
        if (auto sample = read_sample(data); sample) {
            total_samples++;
            if (count_matches(*sample, instruction_set) >= 3) {
                over_three++;
            }
        }
    }
    std::cout << "Samples with over 3 matches: " << over_three << '\n';
    std::cout << "Total samples: " << total_samples << '\n';
}
