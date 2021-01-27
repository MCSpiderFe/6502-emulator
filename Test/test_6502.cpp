#include "test_6502.h"
#include "main_6502.h"

CPU test_cpu;
Mem test_mem;

TEST_RES m6502_test_runInlineProgram()
{
	test_cpu.Reset(test_mem);

	test_mem[0xFFFC] = CPU::INS_JSR;
	test_mem[0xFFFD] = 0x42;
	test_mem[0xFFFE] = 0x42;
	test_mem[0x4242] = CPU::INS_LDA_IM;
	test_mem[0x4243] = 0xFF;

	test_cpu.Exec(8, test_mem);

	EXPECT(test_cpu.A, 0xFF);
}

TEST_RES m6502_test_LDAImToAReg()
{
	test_cpu.Reset(test_mem);

	test_mem[0xFFFC] = CPU::INS_LDA_IM;
	test_mem[0xFFFD] = 0x84;

	test_cpu.Exec(2, test_mem);

	EXPECT(test_cpu.A, 0x84);
}

TEST_RES m6502_test_LDAZpToAReg()
{
	test_cpu.Reset(test_mem);

	test_mem[0xFFFC] = CPU::INS_LDA_ZP;
	test_mem[0xFFFD] = 0x42;
	test_mem[0x0042] = 0x50;

	test_cpu.Exec(3, test_mem);

	EXPECT(test_cpu.A, 0x50);
}

TEST_RES m6502_test_LDAZpXToAReg()
{
	test_cpu.Reset(test_mem);
	test_cpu.X = 5;

	test_mem[0xFFFC] = CPU::INS_LDA_ZPX;
	test_mem[0xFFFD] = 0x42;
	test_mem[0x0047] = 0x64;

	test_cpu.Exec(4, test_mem);

	EXPECT(test_cpu.A, 0x64);
}