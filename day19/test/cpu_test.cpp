#include <gtest/gtest.h>

#include "cpu.hpp"

#include "opcodes.hpp"
#include "program.hpp"

using namespace aoc::cpu;

TEST(cpu_test, cpu_acceptance_test)
{
    Cpu cpu;
    auto program = Program::from_string(
        "#ip 0\n"
        "seti 5 0 1\n"
        "seti 6 0 2\n"
        "addi 0 1 0\n"
        "addr 1 2 3\n"
        "setr 1 0 0\n"
        "seti 8 0 4\n"
        "seti 9 0 5\n"
    );
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[0], 6);
}

class cpu_instruction_test : public ::testing::Test
{
public:
    cpu_instruction_test()
        : cpu()
        , program{{}, {}, {}}
    {}
protected:
    Cpu cpu;
    Program program;
};

TEST_F(cpu_instruction_test, test_addi)
{
    cpu.gpr[1] = 1;
    program.instructions.push_back(Instruction{Op::addi, 1, 2, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 3);
}

TEST_F(cpu_instruction_test, test_addr)
{
    cpu.gpr[1] = 1;
    cpu.gpr[2] = 6;
    program.instructions.push_back(Instruction{Op::addr, 1, 2, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 7);
}

TEST_F(cpu_instruction_test, test_bani)
{
    cpu.gpr[1] = 0xFFFF;
    program.instructions.push_back(Instruction{Op::bani, 1, 0xABAB, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 0xABAB);
}

TEST_F(cpu_instruction_test, test_banr)
{
    cpu.gpr[1] = 0xFFFF;
    cpu.gpr[2] = 0xABAC;
    program.instructions.push_back(Instruction{Op::banr, 1, 2, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 0xABAC);
}

TEST_F(cpu_instruction_test, test_bori)
{
    cpu.gpr[1] = 0x00FF;
    program.instructions.push_back(Instruction{Op::bori, 1, 0xFF00, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 0xFFFF);
}

TEST_F(cpu_instruction_test, test_borr)
{
    cpu.gpr[1] = 0xFF00;
    cpu.gpr[2] = 0x00FF;
    program.instructions.push_back(Instruction{Op::borr, 1, 2, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 0xFFFF);
}

TEST_F(cpu_instruction_test, test_eqir_equal)
{
    cpu.gpr[2] = 0x00FF;
    program.instructions.push_back(Instruction{Op::eqir, 0x00FF, 2, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 1);
}

TEST_F(cpu_instruction_test, test_eqir_not_equal)
{
    cpu.gpr[2] = 0x00AA;
    program.instructions.push_back(Instruction{Op::eqir, 0x00FF, 2, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 0);
}

TEST_F(cpu_instruction_test, test_eqri_equal)
{
    cpu.gpr[2] = 0x00FF;
    program.instructions.push_back(Instruction{Op::eqri, 2, 0x00FF, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 1);
}

TEST_F(cpu_instruction_test, test_eqri_not_equal)
{
    cpu.gpr[2] = 0x00FF;
    program.instructions.push_back(Instruction{Op::eqri, 2, 0x00AA, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 0);
}

TEST_F(cpu_instruction_test, test_eqrr_equal)
{
    cpu.gpr[2] = 0x00FF;
    cpu.gpr[3] = 0x00FF;
    program.instructions.push_back(Instruction{Op::eqrr, 2, 3, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 1);
}

TEST_F(cpu_instruction_test, test_eqrr_not_equal)
{
    cpu.gpr[2] = 0x00FF;
    cpu.gpr[3] = 0x00AA;
    program.instructions.push_back(Instruction{Op::eqrr, 2, 3, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 0);
}

TEST_F(cpu_instruction_test, test_gtir_greater)
{
    cpu.gpr[3] = 3;
    program.instructions.push_back(Instruction{Op::gtir, 4, 3, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 1);
}

TEST_F(cpu_instruction_test, test_gtir_less)
{
    cpu.gpr[3] = 5;
    program.instructions.push_back(Instruction{Op::gtir, 2, 3, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 0);
}

TEST_F(cpu_instruction_test, test_gtri_greater)
{
    cpu.gpr[2] = 5;
    program.instructions.push_back(Instruction{Op::gtri, 2, 4, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 1);
}

TEST_F(cpu_instruction_test, test_gtri_less)
{
    cpu.gpr[2] = 1;
    program.instructions.push_back(Instruction{Op::gtri, 2, 3, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 0);
}

TEST_F(cpu_instruction_test, test_gtrr_greater)
{
    cpu.gpr[2] = 4;
    cpu.gpr[3] = 3;
    program.instructions.push_back(Instruction{Op::gtrr, 2, 3, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 1);
}

TEST_F(cpu_instruction_test, test_gtrr_less)
{
    cpu.gpr[2] = 2;
    cpu.gpr[3] = 3;
    program.instructions.push_back(Instruction{Op::gtrr, 2, 3, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 0);
}

TEST_F(cpu_instruction_test, test_muli)
{
    cpu.gpr[2] = 2;
    program.instructions.push_back(Instruction{Op::muli, 2, 3, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 6);
}

TEST_F(cpu_instruction_test, test_mulr)
{
    cpu.gpr[2] = 2;
    cpu.gpr[3] = 4;
    program.instructions.push_back(Instruction{Op::mulr, 2, 3, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 8);
}

TEST_F(cpu_instruction_test, test_seti)
{
    program.instructions.push_back(Instruction{Op::seti, 5, 0, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 5);
}

TEST_F(cpu_instruction_test, test_setr)
{
    cpu.gpr[2] = 5;
    program.instructions.push_back(Instruction{Op::setr, 2, 0, 3});
    cpu.run_program(program);
    EXPECT_EQ(cpu.gpr[3], 5);
}
