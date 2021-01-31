#include "../6502_pch.h"

static void VerifyUnmodifiedFlagsFromLDA(const CPU& cpu, const CPU& copy)
{
	EXPECT_EQ(cpu.C, copy.C);
	EXPECT_EQ(cpu.I, copy.I);
	EXPECT_EQ(cpu.D, copy.D);
	EXPECT_EQ(cpu.B, copy.B);
	EXPECT_EQ(cpu.V, copy.V);
}

TEST_F(m6502_test_CPUDoesNothingOnZeroCycles)
{
	constexpr s32 NUM_CYCLES = 0;
	s32 cyclesUsed = cpu.Exec(NUM_CYCLES, mem);
	EXPECT_RET(cyclesUsed, 0);
}

TEST_F(m6502_test_runInlineProgram)
{
	CPU cpu_copy = cpu;

	mem[0xFFFC] = CPU::INS_JSR;
	mem[0xFFFD] = 0x42;
	mem[0xFFFE] = 0x42;
	mem[0x4242] = CPU::INS_LDA_IM;
	mem[0x4243] = 0xFF;

	s32 cyclesUsed = cpu.Exec(8, mem);

	EXPECT_FALSE(cpu.Z);
	EXPECT_TRUE(cpu.N);
	EXPECT_EQ(cyclesUsed, 8);
	VerifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
	EXPECT_RET(cpu.A, 0xFF);
}

TEST_F(m6502_test_LDAImToAReg)
{
	CPU cpu_copy = cpu;

	mem[0xFFFC] = CPU::INS_LDA_IM;
	mem[0xFFFD] = 0x84;

	s32 cyclesUsed = cpu.Exec(2, mem);

	EXPECT_FALSE(cpu.Z);
	EXPECT_TRUE(cpu.N);
	EXPECT_EQ(cyclesUsed, 2);
	VerifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
	EXPECT_RET(cpu.A, 0x84);
}

TEST_F(m6502_test_LDAZpToAReg)
{
	CPU cpu_copy = cpu;

	mem[0xFFFC] = CPU::INS_LDA_ZP;
	mem[0xFFFD] = 0x42;
	mem[0x0042] = 0x50;

	s32 cyclesUsed = cpu.Exec(3, mem);

	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	EXPECT_EQ(cyclesUsed, 3);
	VerifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
	EXPECT_RET(cpu.A, 0x50);
}

TEST_F(m6502_test_LDAZpXToAReg)
{
	CPU cpu_copy = cpu;

	cpu.X = 5;

	mem[0xFFFC] = CPU::INS_LDA_ZPX;
	mem[0xFFFD] = 0x42;
	mem[0x0047] = 0x64;

	s32 cyclesUsed = cpu.Exec(4, mem);

	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	EXPECT_EQ(cyclesUsed, 4);
	VerifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
	EXPECT_RET(cpu.A, 0x64);
}

TEST_F(m6502_test_LDAZpToARegWhenWrap)
{
	CPU cpu_copy = cpu;

	cpu.X = 0xFF;

	mem[0xFFFC] = CPU::INS_LDA_ZPX;
	mem[0xFFFD] = 0x80;
	mem[0x007F] = 0x64;

	s32 cyclesUsed = cpu.Exec(4, mem);

	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	EXPECT_EQ(cyclesUsed, 4);
	VerifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
	EXPECT_RET(cpu.A, 0x64);
}