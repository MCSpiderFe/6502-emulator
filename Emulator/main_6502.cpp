#include <stdio.h>
#include <stdlib.h>

//reference:
//http://www.obelisk.me.uk/6502/

struct CPU
{
	using BYTE = unsigned char; //8 bit value
	using WORD = unsigned short; //16 bit value

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

};

int main()
{
	CPU cpu;
	return 0;
}