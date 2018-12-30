#include "opcodes.hpp"

#include <gtest/gtest.h>

using namespace aoc::cpu;

TEST(opcodes_test, opname_to_opcode_all_valid_opnames)
{
    EXPECT_EQ(*opname_to_opcode("addi"), Op::addi);
    EXPECT_EQ(*opname_to_opcode("addr"), Op::addr);
    EXPECT_EQ(*opname_to_opcode("bani"), Op::bani);
    EXPECT_EQ(*opname_to_opcode("banr"), Op::banr);
    EXPECT_EQ(*opname_to_opcode("bori"), Op::bori);
    EXPECT_EQ(*opname_to_opcode("borr"), Op::borr);
    EXPECT_EQ(*opname_to_opcode("eqir"), Op::eqir);
    EXPECT_EQ(*opname_to_opcode("eqri"), Op::eqri);
    EXPECT_EQ(*opname_to_opcode("eqrr"), Op::eqrr);
    EXPECT_EQ(*opname_to_opcode("gtir"), Op::gtir);
    EXPECT_EQ(*opname_to_opcode("gtri"), Op::gtri);
    EXPECT_EQ(*opname_to_opcode("gtrr"), Op::gtrr);
    EXPECT_EQ(*opname_to_opcode("muli"), Op::muli);
    EXPECT_EQ(*opname_to_opcode("mulr"), Op::mulr);
    EXPECT_EQ(*opname_to_opcode("seti"), Op::seti);
    EXPECT_EQ(*opname_to_opcode("setr"), Op::setr);
}

TEST(opcodes_test, opcode_to_opname_valid_opcode)
{
    EXPECT_EQ(*opcode_to_opname(Op::addi), "addi");
    EXPECT_EQ(*opcode_to_opname(Op::addr), "addr");
    EXPECT_EQ(*opcode_to_opname(Op::bani), "bani");
    EXPECT_EQ(*opcode_to_opname(Op::banr), "banr");
    EXPECT_EQ(*opcode_to_opname(Op::bori), "bori");
    EXPECT_EQ(*opcode_to_opname(Op::borr), "borr");
    EXPECT_EQ(*opcode_to_opname(Op::eqir), "eqir");
    EXPECT_EQ(*opcode_to_opname(Op::eqri), "eqri");
    EXPECT_EQ(*opcode_to_opname(Op::eqrr), "eqrr");
    EXPECT_EQ(*opcode_to_opname(Op::gtir), "gtir");
    EXPECT_EQ(*opcode_to_opname(Op::gtri), "gtri");
    EXPECT_EQ(*opcode_to_opname(Op::gtrr), "gtrr");
    EXPECT_EQ(*opcode_to_opname(Op::muli), "muli");
    EXPECT_EQ(*opcode_to_opname(Op::mulr), "mulr");
    EXPECT_EQ(*opcode_to_opname(Op::seti), "seti");
    EXPECT_EQ(*opcode_to_opname(Op::setr), "setr");
}

TEST(opcodes_test, opname_to_opcode_invalid_op)
{
    EXPECT_FALSE(opname_to_opcode("abcdefg"));
    EXPECT_FALSE(opname_to_opcode("invalid"));
}

TEST(opcodes_test, opcode_to_opname_invalid_op)
{
    EXPECT_FALSE(opcode_to_opname(0));
    EXPECT_FALSE(opcode_to_opname(17));
    EXPECT_FALSE(opcode_to_opname(9001));
}
