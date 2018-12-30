#pragma once
#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "cpu.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace aoc::cpu
{
struct Instruction
{
    word op;
    word a_in;
    word b_in;
    word c_out;
};

struct ParseError
{
    std::string what;
    std::string linetext;
    int linenumber;
};

struct Program
{
    std::optional<word> ip_reg;
    std::vector<Instruction> instructions;
    std::vector<ParseError> errors;

    std::string str() const;

    static Program from_stream(std::istream& infile);
    static Program from_string(const std::string& str);

    void serialize(std::ostream& os) const;
    static Program unserialize(std::istream& is);
};
}

#endif  // PROGRAM_HPP
