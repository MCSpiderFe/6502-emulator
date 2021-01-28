#include "test_6502.h"
#include "main_6502.h"

TEST_RES m6502_test_runInlineProgram(CPU& cpu, Mem& mem)
{
	mem[0xFFFC] = CPU::INS_JSR;
	mem[0xFFFD] = 0x42;
	mem[0xFFFE] = 0x42;
	mem[0x4242] = CPU::INS_LDA_IM;
	mem[0x4243] = 0xFF;

	cpu.Exec(8, mem);

	EXPECT(cpu.A, 0xFF);
}

TEST_RES m6502_test_LDAImToAReg(CPU& cpu, Mem& mem)
{
	mem[0xFFFC] = CPU::INS_LDA_IM;
	mem[0xFFFD] = 0x84;

	cpu.Exec(2, mem);

	EXPECT(cpu.A, 0x84);
}

TEST_RES m6502_test_LDAZpToAReg(CPU& cpu, Mem& mem)
{
	mem[0xFFFC] = CPU::INS_LDA_ZP;
	mem[0xFFFD] = 0x42;
	mem[0x0042] = 0x50;

	cpu.Exec(3, mem);

	EXPECT(cpu.A, 0x50);
}

TEST_RES m6502_test_LDAZpXToAReg(CPU& cpu, Mem& mem)
{
	cpu.X = 5;

	mem[0xFFFC] = CPU::INS_LDA_ZPX;
	mem[0xFFFD] = 0x42;
	mem[0x0047] = 0x64;

	cpu.Exec(4, mem);

	EXPECT(cpu.A, 0x64);
}