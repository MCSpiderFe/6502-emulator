#include <stdio.h>
#include <stdlib.h>

//reference:
//http://www.obelisk.me.uk/6502/

using BYTE = unsigned char; //8 bit value
using WORD = unsigned short; //16 bit value

using u32 = unsigned int;

struct Mem
{
	static constexpr u32 MAX_MEM = 1024 * 64;
	BYTE Data[MAX_MEM];

	void Init()
	{
		for (u32 i = 0; i < MAX_MEM; i++) {
			Data[i] = 0;
		}
	}

	//read 1 byte from memory
	BYTE operator[](u32 addr) const
	{
		return Data[addr];
	}

	BYTE& operator[](u32 addr)
	{
		return Data[addr];
	}

	void WriteWord(WORD data, u32 addr, u32& cycles)
	{
		Data[addr] = data & 0xFF;
		Data[addr + 1] = (data >> 8);
		cycles -= 2;
	}
};

struct CPU
{
	WORD PC; //Program Counter
	WORD SP; //Stack Pointer

	BYTE A, X, Y; //registers

	//status flags
	BYTE C : 1; //Carry Flag
	BYTE Z : 1; //Zero Flag
	BYTE I : 1; //Interrupt Disable
	BYTE D : 1; //Decimal Mode
	BYTE B : 1; //Break Command
	BYTE V : 1; //Overflow Flag
	BYTE N : 1; //Negative Flag

	void Reset(Mem& memory)
	{
		PC = 0xFFFC; //set start point in memory
		SP = 0x0100;
		C = Z = I = D = B = V = N = 0;
		A = X = Y = 0;
		memory.Init();
	}

	BYTE FetchByte(u32& cycles, Mem& memory)
	{
		BYTE data = memory[PC];
		PC++;
		cycles--;
		return data;
	}

	WORD FetchWord(u32& cycles, Mem& memory)
	{
		//6502 is little endian
		WORD data = memory[PC];
		PC++;

		data |= (memory[PC] << 8);
		PC++;
		cycles -= 2;
		return data;
	}

	BYTE ReadByte(u32& cycles, BYTE addr, Mem& memory)
	{
		BYTE data = memory[addr];
		cycles--;
		return data;
	}

	static constexpr BYTE	
		INS_LDA_IM = 0xA9,
		INS_LDA_ZP = 0xA5,
		INS_LDA_ZPX = 0xB5,
		INS_JSR = 0x20;

	void LDASetStatus()
	{
		Z = (A == 0);
		N = (A & 0b10000000) > 0;
	}

	void Exec(u32 cycles, Mem& memory)
	{
		while (cycles > 0) {
			BYTE instr = FetchByte(cycles, memory);
			switch (instr) {
			case INS_LDA_IM:
			{
				BYTE value = FetchByte(cycles, memory);
				A = value;
				LDASetStatus();
			} break;
			case INS_LDA_ZP:
			{
				BYTE zeroPageAddr = FetchByte(cycles, memory);
				A = ReadByte(cycles, zeroPageAddr, memory);
				LDASetStatus();
			} break;
			case INS_LDA_ZPX:
			{
				BYTE zeroPageAddr = FetchByte(cycles, memory);
				zeroPageAddr += X;
				cycles--;
				A = ReadByte(cycles, zeroPageAddr, memory);
				LDASetStatus();
			} break;
			case INS_JSR:
			{
				WORD subAddr = FetchWord(cycles, memory);
				memory.WriteWord(PC - 1, SP, cycles);
				PC = subAddr;
				cycles--;
			} break;
			default:
			{
				printf("Instruction not handled %d\n", instr);
			}break;
			}
		}
	}
};

int main()
{
	Mem mem;
	CPU cpu;
	cpu.Reset(mem);

	//test program
	mem[0xFFFC] = CPU::INS_JSR;
	mem[0xFFFD] = 0x42;
	mem[0xFFFE] = 0x42;
	mem[0x4242] = CPU::INS_LDA_IM;
	mem[0x4243] = 0xFF;

	//execute the program
	cpu.Exec(9, mem);

	return 0;
}