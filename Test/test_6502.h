#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define ASSERT(b, ...) {if(!b) {printf(__VA_ARGS__);}} 
#define EXPECT(a, b) {if(a == b) {printf("[  OK  ] test succeeded.\n"); return true;} else {printf("[FAILED] the value was %d, should be %d\n", a, b); return false;}}

using TEST_RES = bool;
typedef TEST_RES (*f)();

TEST_RES m6502_test_runInlineProgram();
TEST_RES m6502_test_LDAImToAReg();
TEST_RES m6502_test_LDAZpToAReg();
TEST_RES m6502_test_LDAZpXToAReg();

struct Test
{
	f tests[4] = {&m6502_test_runInlineProgram, &m6502_test_LDAImToAReg, &m6502_test_LDAZpToAReg, &m6502_test_LDAZpXToAReg };

	bool RunTests()
	{
		int failedTests = 0;
		int numTests = sizeof(tests) / sizeof(f);
		printf("[TESTS ] Running %d tests\n", numTests);

		//run all tests
		for (auto test_func : tests) {
			if (!test_func()) {
				ASSERT(false, "Test Failed\n");
				failedTests++;
			}
		}

		if (failedTests != 0) {
			ASSERT(false, "[FAILED] %d tests from %d tests failed\n", failedTests, numTests);
			exit(-1);
		}
		else {
			printf("[PASSED] %d tests.\n", numTests);
		}
		return true;
	}
};





