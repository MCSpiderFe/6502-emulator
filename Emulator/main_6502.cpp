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
		PC = 0xFFFC;
		SP = 0x0100;
		C = Z = I = D = B = V = N = 0;
		A = X = Y = 0;
		memory.Init();
	}
};

int main()
{
	Mem mem;
	CPU cpu;
	cpu.Reset(mem);
	return 0;
}