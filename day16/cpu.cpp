//  g++ -std=c++17 -O3 -Wall cpu.cpp -c -o cpu.o

#include "cpu.hpp"

#include <algorithm>
#include <map>
#include <iostream>

InstructionFunc* str_to_instruction(std::string str)
{
    if (instruction_set.count(str) == 0) {
        return &unknown_instruction;
    }
    return instruction_set.at(str);
}

// addr (add register) stores into register C the result of adding
// register A and register B.
void addr(Cpu& cpu, uint8_t a_in, uint8_t b_in, uint8_t c_out)
{
    cpu.gpr[c_out] = cpu.gpr[a_in] + cpu.gpr[b_in];
}

// addi (add immediate) stores into register C the result of adding
// register A and value B.
void addi(Cpu& cpu, uint8_t a_in, uint8_t b_in, uint8_t c_out)
{
    cpu.gpr[c_out] = cpu.gpr[a_in] + b_in;
}

// mulr (multiply register) stores into register C the result of
// multiplying register A and register B.
void mulr(Cpu& cpu, uint8_t a_in, uint8_t b_in, uint8_t c_out)
{
    cpu.gpr[c_out] = cpu.gpr[a_in] * cpu.gpr[b_in];
}

// muli (multiply immediate) stores into register C the result
// of multiplying register A and value B.
void muli(Cpu& cpu, uint8_t a_in, uint8_t b_in, uint8_t c_out)
{
    cpu.gpr[c_out] = cpu.gpr[a_in] * b_in;
}

// banr (bitwise AND register) stores into register C the result of the bitwise
// AND of register A and register B.
void banr(Cpu& cpu, uint8_t a_in, uint8_t b_in, uint8_t c_out)
{
    cpu.gpr[c_out] = (cpu.gpr[a_in] & cpu.gpr[b_in]);
}

// bani (bitwise AND immediate) stores into register C the result of the
// bitwise AND of register A and value B.
void bani(Cpu& cpu, uint8_t a_in, uint8_t b_in, uint8_t c_out)
{
    cpu.gpr[c_out] = (cpu.gpr[a_in] & b_in);
}

// borr (bitwise OR register) stores into register C the result of the bitwise
// OR of register A and register B.
void borr(Cpu& cpu, uint8_t a_in, uint8_t b_in, uint8_t c_out)
{
    cpu.gpr[c_out] = (cpu.gpr[a_in] | cpu.gpr[b_in]);
}

// bori (bitwise OR immediate) stores into register C the result of the bitwise
// OR of register A and value B.
void bori(Cpu& cpu, uint8_t a_in, uint8_t b_in, uint8_t c_out)
{
    cpu.gpr[c_out] = (cpu.gpr[a_in] | b_in);
}

// setr (set register) copies the contents of register A into register C.
// (Input B is ignored.)
void setr(Cpu& cpu, uint8_t a_in, uint8_t, uint8_t c_out)
{
    cpu.gpr[c_out] = cpu.gpr[a_in];
}

// seti (set immediate) stores value A into register C.
// (Input B is ignored.)
void seti(Cpu& cpu, uint8_t a_in, uint8_t, uint8_t c_out)
{
    cpu.gpr[c_out] = a_in;
}

// gtir (greater-than immediate/register) sets register C to 1 if value A is
// greater than register B. Otherwise, register C is set to 0.
void gtir(Cpu& cpu, uint8_t a_in, uint8_t b_in, uint8_t c_out)
{
    cpu.gpr[c_out] = (a_in > cpu.gpr[b_in] ? 0x01 : 0x00);
}

// gtri (greater-than register/immediate) sets register C to 1 if register A
// is greater than value B. Otherwise, register C is set to 0.
void gtri(Cpu& cpu, uint8_t a_in, uint8_t b_in, uint8_t c_out)
{
    cpu.gpr[c_out] = (cpu.gpr[a_in] > b_in ? 0x01 : 0x00);
}

// gtrr (greater-than register/register) sets register C to 1 if register A
// is greater than register B. Otherwise, register C is set to 0.
void gtrr(Cpu& cpu, uint8_t a_in, uint8_t b_in, uint8_t c_out)
{
    cpu.gpr[c_out] = (cpu.gpr[a_in] > cpu.gpr[b_in] ? 0x01 : 0x00);
}

// eqir (equal immediate/register) sets register C to 1 if value A is equal to
// register B. Otherwise, register C is set to 0.
void eqir(Cpu& cpu, uint8_t a_in, uint8_t b_in, uint8_t c_out)
{
    cpu.gpr[c_out] = (a_in == cpu.gpr[b_in] ? 0x01 : 0x00);
}

// eqri (equal register/immediate) sets register C to 1 if register A is equal
// to value B. Otherwise, register C is set to 0.
void eqri(Cpu& cpu, uint8_t a_in, uint8_t b_in, uint8_t c_out)
{
    cpu.gpr[c_out] = (cpu.gpr[a_in] == b_in ? 0x01 : 0x00);
}

// eqrr (equal register/register) sets register C to 1 if register A is equal
// to register B. Otherwise, register C is set to 0.
void eqrr(Cpu& cpu, uint8_t a_in, uint8_t b_in, uint8_t c_out)
{
    cpu.gpr[c_out] = (cpu.gpr[a_in] == cpu.gpr[b_in] ? 0x01 : 0x00);
}

void unknown_instruction(Cpu& cpu, uint8_t a_in, uint8_t b_in, uint8_t c_out)
{
    std::cout << "Unknown instruction at " << cpu.pc << '\n';
}
