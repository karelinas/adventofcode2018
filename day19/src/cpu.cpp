#include "cpu.hpp"

#include "opcodes.hpp"
#include "program.hpp"

#include <atomic>
#include <iostream>
#include <mutex>

#include <signal.h>

std::ostream& operator<<(std::ostream& os, const aoc::cpu::Cpu& cpu)
{
    os << "gpr[ ";
    for (auto& reg : cpu.gpr) {
        os << reg << " ";
    }
    os << "]  ip[ " << *cpu.ip << " ]";
    return os;
}

namespace aoc::cpu
{
namespace
{
std::atomic<bool> exit_requested = false;
}

void siginthandler(int)
{
    exit_requested = true;
}

ExitStatus Cpu::run_program(const Program& program, std::ostream* out)
{
    static std::once_flag signal_flag;
    std::call_once(signal_flag, [](){
        signal(SIGINT, siginthandler);
    });

    exit_requested = false;
    if (program.ip_reg) {
        if (*program.ip_reg >= static_cast<word>(gpr.size())) {
            // TODO: log error
            std::cerr << "Ip reg too big\n";
            return ExitStatus::Error;
        }

        ip = &gpr[*program.ip_reg];
    }

    while (*ip < static_cast<word>(program.instructions.size())) {
        auto instr = program.instructions[*ip];

// Helper macros for the switch below
#define A_IN     instr.a_in
#define A_IN_REG gpr[A_IN]
#define B_IN     instr.b_in
#define B_IN_REG gpr[B_IN]
#define C_OUT    gpr[instr.c_out]

        switch (instr.op) {
            case addi: C_OUT = A_IN_REG + B_IN; break;
            case addr: C_OUT = A_IN_REG + B_IN_REG; break;
            case bani: C_OUT = (A_IN_REG & B_IN); break;
            case banr: C_OUT = (A_IN_REG & B_IN_REG); break;
            case bori: C_OUT = (A_IN_REG | B_IN); break;
            case borr: C_OUT = (A_IN_REG | B_IN_REG); break;
            case eqir: C_OUT = (A_IN == B_IN_REG ? 1 : 0); break;
            case eqri: C_OUT = (A_IN_REG == B_IN ? 1 : 0); break;
            case eqrr: C_OUT = (A_IN_REG == B_IN_REG ? 1 : 0); break;
            case gtir: C_OUT = (A_IN > B_IN_REG ? 1 : 0); break;
            case gtri: C_OUT = (A_IN_REG > B_IN ? 1 : 0); break;
            case gtrr: C_OUT = (A_IN_REG > B_IN_REG ? 1 : 0); break;
            case muli: C_OUT = A_IN_REG * B_IN; break;
            case mulr: C_OUT = A_IN_REG * B_IN_REG; break;
            case seti: C_OUT = A_IN; break;
            case setr: C_OUT = A_IN_REG; break;
            default:
                // TODO log error
                std::cerr << "Invalid instruction\n";
                return ExitStatus::Error;
        }
        (*ip)++;
        cycle_count++;
        if (exit_requested) {
            if (out) {
                (*out) << "\nExiting. Cycle count: " << cycle_count
                       << " State: " << *this << '\n';
            }
            return ExitStatus::Interrupted;
        }
    }
    (*ip)--;
    return ExitStatus::Ok;
}

void Cpu::serialize(std::ostream& os) const
{
    for (auto reg : gpr) {
        os.write(reinterpret_cast<char*>(&reg), sizeof(word));
    }
    auto pc_reg_value = pc_reg;
    auto cycle_count_value = cycle_count;
    os.write(reinterpret_cast<char*>(&pc_reg_value), sizeof(word));
    os.write(reinterpret_cast<char*>(&cycle_count_value), sizeof(word));
}

Cpu Cpu::unserialize(std::istream& is)
{
    Cpu cpu;
    for (auto& reg : cpu.gpr) {
        is.read(reinterpret_cast<char*>(&reg), sizeof(word));
    }
    is.read(reinterpret_cast<char*>(&cpu.pc_reg), sizeof(word));
    is.read(reinterpret_cast<char*>(&cpu.cycle_count), sizeof(word));
    return cpu;
}
}
