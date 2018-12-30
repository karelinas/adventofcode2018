#pragma once
#ifndef OPCODES_HPP
#define OPCODES_HPP

#include "cpu.hpp"

#include <array>
#include <optional>
#include <string>
#include <tuple>

namespace aoc::cpu
{
const word        INVALID_OPCODE = 0;
const std::string INVALID_OPNAME = "invalid_op";

enum Op : word
{
    addi = 1,
    addr = 2,
    bani = 3,
    banr = 4,
    bori = 5,
    borr = 6,
    eqir = 7,
    eqri = 8,
    eqrr = 9,
    gtir = 10,
    gtri = 11,
    gtrr = 12,
    muli = 13,
    mulr = 14,
    seti = 15,
    setr = 16
};

const std::array<std::tuple<std::string, Op>, 16> OPCODES = {
    std::make_tuple("addi",  Op::addi),
    std::make_tuple("addr",  Op::addr),
    std::make_tuple("bani",  Op::bani),
    std::make_tuple("banr",  Op::banr),
    std::make_tuple("bori",  Op::bori),
    std::make_tuple("borr",  Op::borr),
    std::make_tuple("eqir",  Op::eqir),
    std::make_tuple("eqri",  Op::eqri),
    std::make_tuple("eqrr",  Op::eqrr),
    std::make_tuple("gtir",  Op::gtir),
    std::make_tuple("gtri",  Op::gtri),
    std::make_tuple("gtrr",  Op::gtrr),
    std::make_tuple("muli",  Op::muli),
    std::make_tuple("mulr",  Op::mulr),
    std::make_tuple("seti",  Op::seti),
    std::make_tuple("setr",  Op::setr)
};

std::optional<Op>          opname_to_opcode(const std::string& opname);
std::optional<std::string> opcode_to_opname(word opcode);
}
#endif  // OPCODES_HPP