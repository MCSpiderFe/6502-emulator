#include "test_6502.h"

TEST_F(m6502_test_CPUDoesNothingOnZeroCycles)
{
	constexpr s32 NUM_CYCLES = 0;
	s32 cyclesUsed = cpu.Exec(NUM_CYCLES, mem);
	EXPECT_EQ(cyclesUsed, 0);
}

TEST_F(m6502_test_CPUCanExecuteMoreCyclesThanGiven)
{
	mem[0xFFFC] = CPU::INS_LDA_IM;
	mem[0xFFFD] = 0x84;

	constexpr s32 NUM_CYCLES = 1;
	s32 cyclesUsed = cpu.Exec(NUM_CYCLES, mem);

	EXPECT_EQ(cyclesUsed, 2);
}

TEST_F(m6502_test_ExecuteUnknownInstructionNoInfiniteLoop)
{
	mem[0xFFFC] = 0x0; //invalid instruction
	mem[0xFFFD] = 0x0;

	constexpr s32 NUM_CYCLES = 1;
	s32 cyclesUsed = cpu.Exec(NUM_CYCLES, mem);

	EXPECT_EQ(cyclesUsed, NUM_CYCLES);
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
	EXPECT_EQ(cpu.A, 0xFF);
}