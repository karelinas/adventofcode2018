#include <gtest/gtest.h>

#include "opcodes.hpp"
#include "program.hpp"

using namespace aoc::cpu;

TEST(program_test, from_string_single_instruction_valid)
{
    auto program = Program::from_string("borr 1 2 3");

    EXPECT_EQ(program.errors.size(), 0);
    EXPECT_FALSE(program.ip_reg);
    ASSERT_EQ(program.instructions.size(), 1);
    auto instruction = program.instructions[0];
    EXPECT_EQ(instruction.op, Op::borr);
    EXPECT_EQ(instruction.a_in, 1);
    EXPECT_EQ(instruction.b_in, 2);
    EXPECT_EQ(instruction.c_out, 3);
}

TEST(program_test, from_string_single_declaration_valid)
{
    auto program = Program::from_string("#ip 4");

    EXPECT_EQ(program.instructions.size(), 0);
    EXPECT_EQ(program.errors.size(), 0);
    ASSERT_TRUE(program.ip_reg);
    EXPECT_EQ(*program.ip_reg, 4);
}

TEST(program_test, from_string_single_instruction_invalid)
{
    auto program = Program::from_string("invalid 1 2 3");

    EXPECT_EQ(program.instructions.size(), 0);
    EXPECT_EQ(program.errors.size(), 1);
    EXPECT_FALSE(program.ip_reg);
}

TEST(program_test, from_string_single_declaration_invalid)
{
    auto program = Program::from_string("#invalid 42");

    EXPECT_EQ(program.instructions.size(), 0);
    EXPECT_EQ(program.errors.size(), 1);
    EXPECT_FALSE(program.ip_reg);
}

TEST(program_test, from_string_full_program_valid)
{
    auto program = Program::from_string(
        "#ip 2\n"
        "borr 1 2 3\n"
        "addi 4 2 1\n"
        "setr 1 1 1\n"
    );

    EXPECT_EQ(program.ip_reg, 2);
    EXPECT_EQ(program.errors.size(), 0);
    ASSERT_EQ(program.instructions.size(), 3);
    EXPECT_EQ(program.instructions[0].op,    Op::borr);
    EXPECT_EQ(program.instructions[0].a_in,  1);
    EXPECT_EQ(program.instructions[0].b_in,  2);
    EXPECT_EQ(program.instructions[0].c_out, 3);
    EXPECT_EQ(program.instructions[1].op,    Op::addi);
    EXPECT_EQ(program.instructions[1].a_in,  4);
    EXPECT_EQ(program.instructions[1].b_in,  2);
    EXPECT_EQ(program.instructions[1].c_out, 1);
    EXPECT_EQ(program.instructions[2].op,    Op::setr);
    EXPECT_EQ(program.instructions[2].a_in,  1);
    EXPECT_EQ(program.instructions[2].b_in,  1);
    EXPECT_EQ(program.instructions[2].c_out, 1);
}

TEST(program_test, from_string_survives_extra_whitespace)
{
    auto program = Program::from_string(
        "#ip    2         \r\r\n"
        "borr   1     2    3\n"
        "addi 4 \t 2 \t1\t      \n"
        "\n"
        "\n"
        "setr    1   1   1     \n"
    );

    EXPECT_EQ(program.ip_reg, 2);
    EXPECT_EQ(program.errors.size(), 0);
    ASSERT_EQ(program.instructions.size(), 3);
    EXPECT_EQ(program.instructions[0].op,    Op::borr);
    EXPECT_EQ(program.instructions[0].a_in,  1);
    EXPECT_EQ(program.instructions[0].b_in,  2);
    EXPECT_EQ(program.instructions[0].c_out, 3);
    EXPECT_EQ(program.instructions[1].op,    Op::addi);
    EXPECT_EQ(program.instructions[1].a_in,  4);
    EXPECT_EQ(program.instructions[1].b_in,  2);
    EXPECT_EQ(program.instructions[1].c_out, 1);
    EXPECT_EQ(program.instructions[2].op,    Op::setr);
    EXPECT_EQ(program.instructions[2].a_in,  1);
    EXPECT_EQ(program.instructions[2].b_in,  1);
    EXPECT_EQ(program.instructions[2].c_out, 1);
}

TEST(program_test, program_to_string)
{
    Program program{
        3, // ip_reg
        {
            Instruction{Op::addi, 2, 3, 4},
            Instruction{Op::bani, 1, 3, 8},
            Instruction{Op::borr, 4, 6, 3}
        },
        {}
    };
    EXPECT_EQ(program.str(),
        "#ip 3\n"
        "addi 2 3 4\n"
        "bani 1 3 8\n"
        "borr 4 6 3\n"
    );
}
