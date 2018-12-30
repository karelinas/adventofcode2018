#ifndef CPU_HPP
#define CPU_HPP

#include <array>
#include <cstdint>
#include <iostream>

namespace aoc::cpu
{
using word = int64_t;
const word REGISTER_COUNT = 6;

// forward declaration
struct Program;

enum class ExitStatus : word
{
    Ok,
    Error,
    Interrupted
};

struct Cpu
{
    // general purpose registers
    std::array<word, REGISTER_COUNT> gpr;

    // default program counter register
    word pc_reg;

    // ip is bound to an arbitrary register specified in the program
    word* ip;

    // count how many instructions have been run
    word cycle_count;

    Cpu() : gpr(), pc_reg(0), ip(nullptr), cycle_count(0) {
        ip = &pc_reg;
    }

    ExitStatus run_program(const Program& program, std::ostream* out = nullptr);

    void serialize(std::ostream& out) const;
    static Cpu unserialize(std::istream& in);
};
}

std::ostream& operator<<(std::ostream& os, const aoc::cpu::Cpu& cpu);

#endif  // CPU_HPP
