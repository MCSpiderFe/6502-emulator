#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <sys/timeb.h>
#include <ctime>
#include <vector>

#include "../emulator/emulator_6502.h"

#define TEST_F(name) void name(CPU& cpu, Mem& mem)

#define ANSI_COLOR_RESET  "\x1b[0m"
#define ANSI_COLOR_RED(str)    "\x1b[31m" str ANSI_COLOR_RESET
#define ANSI_COLOR_GREEN(str)  "\x1b[32m" str ANSI_COLOR_RESET
#define ANSI_COLOR_YELLOW(str) "\x1b[33m" str ANSI_COLOR_RESET

#define ASSERT(b, op, ...) {if(!b) {Test::currentTestResult = Test::TEST_ERR; op (__VA_ARGS__);} else if(Test::currentTestResult == Test::TEST_NULL) {Test::currentTestResult = Test::TEST_OK;}}
#define EXPECT_EQ(a, b) ASSERT((a == b), printf, ANSI_COLOR_RED("[FAILED]") " the value was %d, should be %d\n", a, b)
#define EXPECT_FALSE(a) ASSERT((a == false), printf, ANSI_COLOR_RED("[FAILED]") " the value %d is not false\n", a)
#define EXPECT_TRUE(a)  ASSERT((a == true), printf, ANSI_COLOR_RED("[FAILED]") "the value %d is not true\n", a)

#define TIMEPOINT std::chrono::milliseconds
#define MAKE_TIMEPOINT() std::chrono::duration_cast<TIMEPOINT>(std::chrono::system_clock::now().time_since_epoch())

TEST_F(m6502_test_runInlineProgram);
TEST_F(m6502_test_LDAImToAReg);
TEST_F(m6502_test_LDAZpToAReg);
TEST_F(m6502_test_LDAZpXToAReg);
TEST_F(m6502_test_LDAZpToARegWhenWrap);
TEST_F(m6502_test_CPUDoesNothingOnZeroCycles);
TEST_F(m6502_test_CPUCanExecuteMoreCyclesThanGiven);
TEST_F(m6502_test_ExecuteUnknownInstructionNoInfiniteLoop);
TEST_F(m6502_test_LDAImZeroToAReg);
TEST_F(m6502_test_ExecuteUnknownInstructionNoInfiniteLoop);

struct Test
{
	typedef void(*testFunction)(CPU& cpu, Mem& mem);
	testFunction tests[10] = {
		&m6502_test_runInlineProgram, 
		&m6502_test_LDAImToAReg, 
		&m6502_test_LDAZpToAReg, 
		&m6502_test_LDAZpXToAReg, 
		&m6502_test_LDAZpToARegWhenWrap, 
		&m6502_test_CPUDoesNothingOnZeroCycles,
		&m6502_test_CPUCanExecuteMoreCyclesThanGiven,
		&m6502_test_ExecuteUnknownInstructionNoInfiniteLoop,
		&m6502_test_LDAImZeroToAReg,
		&m6502_test_ExecuteUnknownInstructionNoInfiniteLoop
	};


	typedef enum
	{
		TEST_ERR = -1,
		TEST_NULL = 0,
		TEST_OK = 1,
	} TEST_RES;
	static TEST_RES currentTestResult;

	bool RunTests();
};






