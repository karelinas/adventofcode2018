// g++ -std=c++17 -O3 -Wall -c solution.cpp -o solution.o
// g++ -std=c++17 -O3 -Wall cpu.cpp -c -o cpu.o
// g++ cpu.o solution.o -o main
// ./main


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

std::ostream& operator<<(std::ostream& os, const Cpu& cpu)
{
    os << "Cpu: ";
    for (auto reg : cpu.gpr) {
        os << static_cast<int>(reg) << " ";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Instruction& instr)
{
    os << "Instr: "
       << static_cast<int>(instr.opcode) << " "
       << static_cast<int>(instr.a_in) << " "
       << static_cast<int>(instr.b_in) << " "
       << static_cast<int>(instr.c_out) << '\n';
    return os;
}

std::ostream& operator<<(std::ostream& os, const Sample& sample)
{
    os << "=============================\n";
    os << "Before: " << sample.cpu_before << '\n';
    os << "Instr: " << sample.instruction << '\n';
    os << "After: " << sample.cpu_after << '\n';
    return os;
}

uint32_t char_to_int(char c)
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
    return Instruction{static_cast<uint32_t>(opcode),
                       static_cast<uint32_t>(a_in),
                       static_cast<uint32_t>(b_in),
                       static_cast<uint32_t>(c_out)};
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

void find_matches(Sample& sample, std::map<uint32_t, std::vector<std::string>>& matches)
{
    // add all instructions to matches if it's the first time we've
    // seen the opcode
    if (matches.count(sample.instruction.opcode) == 0) {
        matches[sample.instruction.opcode] = std::vector<std::string>{};
        std::transform(std::begin(instruction_set), std::end(instruction_set),
                       std::back_inserter(matches[sample.instruction.opcode]),
                       [](decltype(instruction_set)::value_type instr) -> std::string {
                           return instr.first;
                       });
    }
    // test and remove opcodes that produce different results than expected
    matches[sample.instruction.opcode].erase(
        std::remove_if(std::begin(matches[sample.instruction.opcode]),
                       std::end(matches[sample.instruction.opcode]),
                       [&sample](std::string& instr) {
                            auto handler = str_to_instruction(instr);
                            Cpu expected{sample.cpu_before};
                            handler(expected,
                                    sample.instruction.a_in,
                                    sample.instruction.b_in,
                                    sample.instruction.c_out);
                            return !(sample.cpu_after == expected);
                       }),
        std::end(matches[sample.instruction.opcode]));
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

void part1()
{
    std::map<uint32_t, std::vector<std::string>> matches;
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

void part2_find_matches()
{
    // part 2
    std::map<uint32_t, std::vector<std::string>> matches;
    std::ifstream data("data.txt");
    while (data) {
        if (auto sample = read_sample(data); sample) {
            find_matches(*sample, matches);
        }
    }

    for (auto const& [key, val] : matches) {
        std::cout << static_cast<int>(key) << " => [ ";
        for (auto const& instr : val) {
            std::cout << "\"" << instr << "\"" << " ";
        }
        std::cout << "]\n";
    }
}

std::optional<Instruction> read_instruction(std::string line)
{
    if (line.size() < 6) {
        return {};
    }
    return read_instruction_line(line);
}

std::vector<Instruction> read_program(std::string file)
{
    std::ifstream data(file);
    std::vector<Instruction> program;
    std::string line;
    while (std::getline(data, line)) {
        if (auto instr = read_instruction(line); instr) {
            program.push_back(*instr);
        }
    }
    return program;
}

int main()
{
    part1();
    part2_find_matches();

    // solved manually from part2_find_matches() printout:
    // 0 => [ "bani" ]
    // 1 => [ "addr" ]
    // 2 => [ "mulr" ]
    // 3 => [ "addi" ]
    // 4 => [ "gtri" ]
    // 5 => [ "banr" ]
    // 6 => [ "borr" ]
    // 7 => [ "eqri" ]
    // 8 => [ "seti" ]
    // 9 => [ "eqrr" ]
    // 10 => [ "bori" ]
    // 11 => [ "setr" ]
    // 12 => [ "eqir" ]
    // 13 => [ "muli" ]
    // 14 => [ "gtrr" ]
    // 15 => [ "gtir" ]

    Cpu cpu{{0, 0, 0, 0}, 0, read_program("data2.txt")};
    std::cout << "Executing " << cpu.program.size() << " instructions\n";
    run_cpu(cpu, [](uint32_t opcode) -> InstructionFunc* {
            static const std::vector<InstructionFunc*> decoded_instr = {
                &bani,
                &addr,
                &mulr,
                &addi,
                &gtri,
                &banr,
                &borr,
                &eqri,
                &seti,
                &eqrr,
                &bori,
                &setr,
                &eqir,
                &muli,
                &gtrr,
                &gtir
            };
            return decoded_instr[opcode];
        });
    std::cout << cpu << '\n';
}
