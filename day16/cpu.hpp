#ifndef CPU_HPP
#define CPU_HPP

#include <array>
#include <string>
#include <map>
#include <vector>
#include <cstdint>

struct Instruction
{
    uint32_t opcode;
    uint32_t a_in;
    uint32_t b_in;
    uint32_t c_out;
};

struct Cpu
{
    // general purpose registers
    std::array<uint32_t, 4> gpr;

    // program counter
    uint32_t pc;

    std::vector<Instruction> program;

    bool operator==(const Cpu& right)
    {
        return  gpr[0] == right.gpr[0] &&
                gpr[1] == right.gpr[1] &&
                gpr[2] == right.gpr[2] &&
                gpr[3] == right.gpr[3] &&
                pc == right.pc;
    }
};

using InstructionFunc = void(Cpu&, uint32_t, uint32_t, uint32_t);
using InstructionDecoder = InstructionFunc*(uint32_t);

void run_cpu(Cpu& cpu, InstructionDecoder* decoder);

InstructionFunc* str_to_instruction(std::string str);

void addr(Cpu& cpu, uint32_t a_in, uint32_t b_in, uint32_t c_out);
void addi(Cpu& cpu, uint32_t a_in, uint32_t b_in, uint32_t c_out);
void mulr(Cpu& cpu, uint32_t a_in, uint32_t b_in, uint32_t c_out);
void muli(Cpu& cpu, uint32_t a_in, uint32_t b_in, uint32_t c_out);
void banr(Cpu& cpu, uint32_t a_in, uint32_t b_in, uint32_t c_out);
void bani(Cpu& cpu, uint32_t a_in, uint32_t b_in, uint32_t c_out);
void borr(Cpu& cpu, uint32_t a_in, uint32_t b_in, uint32_t c_out);
void bori(Cpu& cpu, uint32_t a_in, uint32_t b_in, uint32_t c_out);
void setr(Cpu& cpu, uint32_t a_in, uint32_t, uint32_t c_out);
void seti(Cpu& cpu, uint32_t a_in, uint32_t, uint32_t c_out);
void gtir(Cpu& cpu, uint32_t a_in, uint32_t b_in, uint32_t c_out);
void gtri(Cpu& cpu, uint32_t a_in, uint32_t b_in, uint32_t c_out);
void gtrr(Cpu& cpu, uint32_t a_in, uint32_t b_in, uint32_t c_out);
void eqir(Cpu& cpu, uint32_t a_in, uint32_t b_in, uint32_t c_out);
void eqri(Cpu& cpu, uint32_t a_in, uint32_t b_in, uint32_t c_out);
void eqrr(Cpu& cpu, uint32_t a_in, uint32_t b_in, uint32_t c_out);
void unknown_instruction(Cpu& cpu, uint32_t a_in, uint32_t b_in, uint32_t c_out);

const std::map<std::string, InstructionFunc*> instruction_set = {
    { "addr", &addr },
    { "addi", &addi },
    { "mulr", &mulr },
    { "muli", &muli },
    { "banr", &banr },
    { "bani", &bani },
    { "borr", &borr },
    { "bori", &bori },
    { "setr", &setr },
    { "seti", &seti },
    { "gtir", &gtir },
    { "gtri", &gtri },
    { "gtrr", &gtrr },
    { "eqir", &eqir },
    { "eqri", &eqri },
    { "eqrr", &eqrr }
};

#endif  // CPU_HPP
