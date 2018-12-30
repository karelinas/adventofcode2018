#include "opcodes.hpp"

#include <algorithm>

namespace aoc::cpu
{
std::optional<Op> opname_to_opcode(const std::string& opname)
{
    auto found = std::find_if(std::begin(OPCODES), std::end(OPCODES),
                              [&opname](const std::tuple<std::string, word>& op) {
                                  return std::get<0>(op) == opname;
                              });
    if (found == std::end(OPCODES))
        return {};
    else
        return std::get<1>(*found);
}

std::optional<std::string> opcode_to_opname(word opcode)
{
    auto found = std::find_if(std::begin(OPCODES), std::end(OPCODES),
                              [&opcode](const std::tuple<std::string, word>& op) {
                                  return std::get<1>(op) == opcode;
                              });
    if (found == std::end(OPCODES))
        return {};
    else
        return std::get<0>(*found);
}
}
