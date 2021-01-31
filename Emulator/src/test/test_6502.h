#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <sys/timeb.h>
#include <ctime>
#include "../emulator/emulator_6502.h"

typedef enum
{
	TEST_ERR = -1,
	TEST_NULL = 0,
	TEST_OK = 1,
} TEST_RES;
typedef TEST_RES(*function)(CPU& cpu, Mem& mem);

#define ANSI_COLOR_RESET  "\x1b[0m"
#define ANSI_COLOR_RED(str)    "\x1b[31m" str ANSI_COLOR_RESET
#define ANSI_COLOR_GREEN(str)  "\x1b[32m" str ANSI_COLOR_RESET
#define ANSI_COLOR_YELLOW(str) "\x1b[33m" str ANSI_COLOR_RESET

#define ASSERT(b, op, ...) {if(!b) {op (__VA_ARGS__);}}
#define EXPECT_EQ(a, b) ASSERT((a == b), printf, ANSI_COLOR_RED("[FAILED]") " the value was %d, should be %d\n", a, b)
#define EXPECT_FALSE(a) ASSERT((a == false), printf, ANSI_COLOR_RED("[FAILED]") " the value %d is not false\n", a)
#define EXPECT_TRUE(a)  ASSERT((a == true), printf, ANSI_COLOR_RED("[FAILED]") "the value %d is not true\n", a)
#define EXPECT_RET(a, b) {if(a == b) {return TEST_OK;} \
	else {printf(ANSI_COLOR_RED("[FAILED]") " the value was %d, should be %d\n", a, b); return TEST_ERR;}}

#define TIMEPOINT std::chrono::milliseconds
#define MK_TP(time) std::chrono::duration_cast<TIMEPOINT>(std::chrono::system_clock::now().time())

#define TEST_F(name) TEST_RES name(CPU& cpu, Mem& mem)

TEST_F(m6502_test_runInlineProgram);
TEST_F(m6502_test_LDAImToAReg);
TEST_F(m6502_test_LDAZpToAReg);
TEST_F(m6502_test_LDAZpXToAReg);
TEST_F(m6502_test_LDAZpToARegWhenWrap);
TEST_F(m6502_test_CPUDoesNothingOnZeroCycles);

struct Test
{
	function tests[6] = {&m6502_test_runInlineProgram, &m6502_test_LDAImToAReg, &m6502_test_LDAZpToAReg, &m6502_test_LDAZpXToAReg, &m6502_test_LDAZpToARegWhenWrap, &m6502_test_CPUDoesNothingOnZeroCycles };

	bool RunTests()
	{
		int failedTests = 0;
		int numTests = sizeof(tests) / sizeof(function);
		TIMEPOINT start = MK_TP(time_since_epoch);
		printf(ANSI_COLOR_YELLOW("[======]") " Running %d tests\n", numTests);

		Mem test_mem;
		CPU test_cpu;

		//run all tests
		for (auto test : tests) {
			test_cpu.Reset(test_mem);
			TEST_RES result = test(test_cpu, test_mem);

			switch (result) {
			case TEST_OK:
				printf(ANSI_COLOR_GREEN("[  OK  ]") " test succeeded.\n");
				break;
			case TEST_ERR:
				ASSERT(test, printf, "Test Failed\n");
				failedTests++;
				break;
			case TEST_NULL:
			default:
				printf(ANSI_COLOR_YELLOW("[ NULL ]") " test has no result.\n");
			}
		}

		if (failedTests != 0) {
			ASSERT(false, printf, ANSI_COLOR_RED("[FAILED]") " %d tests from %d tests failed\n", failedTests, numTests);
			exit(-1);
		}
		else {
			TIMEPOINT end = MK_TP(time_since_epoch);
			printf(ANSI_COLOR_GREEN("[PASSED]") " %d tests in %lld ms.\n", numTests,(end - start).count());
		}
		return true;
	}
};





