#include "program.hpp"

#include "opcodes.hpp"

#include <cstdlib>
#include <optional>
#include <regex>
#include <sstream>

#include <iostream>

namespace aoc::cpu
{
namespace
{
bool str_starts_with(const std::string& str, const std::string& prefix)
{
    return str.rfind(prefix, 0) == 0;
}

bool is_declaration(const std::string& str)
{
    return str_starts_with(str, "#");
}

bool is_all_whitespace(const std::string& str)
{
    return str.find_first_not_of("\r\t ") == std::string::npos;
}

std::optional<word> parse_ip_declaration(const std::string& line)
{
    static const std::regex ip_rx("^#ip\\s+(\\d+)\\s*$");

    std::smatch matches;
    if (!std::regex_match(line, matches, ip_rx)) {
        return {};
    }

    return std::atoi(matches[1].str().c_str());
}

std::optional<Instruction> parse_instruction(const std::string& line)
{
    static const std::regex instr_rx("^(\\w+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s*$");
    std::smatch matches;
    if (!std::regex_match(line, matches, instr_rx)) {
        return {};
    }

    auto op = opname_to_opcode(matches[1].str());
    if (!op) {
        return {};
    }

    return Instruction{*op,
                       static_cast<word>(std::stoi(matches[2].str())),
                       static_cast<word>(std::stoi(matches[3].str())),
                       static_cast<word>(std::stoi(matches[4].str()))};
}
}

Program Program::from_stream(std::istream& stream)
{
    Program program{{}, {}, {}};
    std::string line;
    int linenumber = 0;
    while (std::getline(stream, line)) {
        linenumber++;
        if (is_all_whitespace(line))
            continue;
        if (is_declaration(line)) {
            if (auto ip_reg = parse_ip_declaration(line); ip_reg)
                program.ip_reg = *ip_reg;
            else
                program.errors.push_back(ParseError{"Invalid declaration", line, linenumber});
        } else {
            if (auto instr = parse_instruction(line); instr)
                program.instructions.push_back(*instr);
            else
                program.errors.push_back(ParseError{"Invalid instruction", line, linenumber});
        }
    }
    return program;
}

Program Program::from_string(const std::string& str)
{
    std::istringstream iss(str);
    return Program::from_stream(iss);
}

std::string Program::str() const
{
    std::ostringstream oss;
    if (ip_reg)
        oss << "#ip " << *ip_reg << '\n';
    for (const auto& instr : instructions) {
        oss << *opcode_to_opname(instr.op) << ' '
            << instr.a_in << ' '
            << instr.b_in << ' '
            << instr.c_out << '\n';
    }
    return oss.str();
}

void Program::serialize(std::ostream& os) const
{
    if (ip_reg) {
        bool t = true;
        os.write(reinterpret_cast<char*>(&t), sizeof(bool));
        auto ip_reg_value = *ip_reg;
        os.write(reinterpret_cast<char*>(&ip_reg_value), sizeof(word));
    } else {
        bool f = false;
        os.write(reinterpret_cast<char*>(&f), sizeof(bool));
    }

    auto size = instructions.size();
    os.write(reinterpret_cast<char*>(&size), sizeof(decltype(instructions)::size_type));
    for (auto instr : instructions) {
        os.write(reinterpret_cast<char*>(&instr), sizeof(Instruction));
    }
}

Program Program::unserialize(std::istream& is)
{
    Program program{{}, {}, {}};

    // read ip_reg
    bool has_ip_reg = false;
    is.read(reinterpret_cast<char*>(&has_ip_reg), sizeof(bool));
    if (has_ip_reg) {
        word ip_reg;
        is.read(reinterpret_cast<char*>(&ip_reg), sizeof(word));
        program.ip_reg = ip_reg;
    }

    // read instructions
    decltype(program.instructions)::size_type size = 0;
    is.read(reinterpret_cast<char*>(&size), sizeof(decltype(program.instructions)::size_type));
    for (int i = 0; i < size; i++) {
        Instruction instr;
        is.read(reinterpret_cast<char*>(&instr), sizeof(Instruction));
        program.instructions.push_back(instr);
    }
    return program;
}
}
