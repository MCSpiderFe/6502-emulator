#include "test_6502.h"

static void VerifyUnmodifiedFlagsFromLDA(const CPU& cpu, const CPU& copy)
{
	EXPECT_EQ(cpu.C, copy.C);
	EXPECT_EQ(cpu.I, copy.I);
	EXPECT_EQ(cpu.D, copy.D);
	EXPECT_EQ(cpu.B, copy.B);
	EXPECT_EQ(cpu.V, copy.V);
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
	EXPECT_EQ(cpu.A, 0x84);
}

TEST_F(m6502_test_LDAImZeroToAReg)
{
	CPU cpu_copy = cpu;
	cpu.A = 0x44;
	mem[0xFFFC] = CPU::INS_LDA_IM;
	mem[0xFFFD] = 0x00;

	s32 cyclesUsed = cpu.Exec(2, mem);

	EXPECT_TRUE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	EXPECT_EQ(cyclesUsed, 2);
	VerifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
	EXPECT_EQ(cpu.A, 0x0);
}

TEST_F(m6502_test_LDAImAffectsZeroFlag)
{

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
	EXPECT_EQ(cpu.A, 0x50);
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
	EXPECT_EQ(cpu.A, 0x64);
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
	EXPECT_EQ(cpu.A, 0x64);
}

TEST_F(m6502_test_LDAAbsoluteToAReg)
{
	CPU cpu_copy = cpu;

	mem[0xFFFC] = CPU::INS_LDA_ABS;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44; //0x4480
	mem[0x4480] = 0x64;

	constexpr s32 EXPECTED_CYCLES = 4;
	s32 cyclesUsed = cpu.Exec(EXPECTED_CYCLES, mem);

	EXPECT_FALSE(cpu.Z);
	EXPECT_FALSE(cpu.N);
	EXPECT_EQ(cyclesUsed, EXPECTED_CYCLES);
	VerifyUnmodifiedFlagsFromLDA(cpu, cpu_copy);
	EXPECT_EQ(cpu.A, 0x64);
}