#include "main_6502.h"

int main()
{
	Test test;
	test.RunTests();

	Mem mem;
	CPU cpu;
	cpu.Reset(mem);

	return 0;
}