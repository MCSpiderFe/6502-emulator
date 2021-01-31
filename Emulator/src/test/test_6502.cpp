#include "test_6502.h"

Test::TEST_RES Test::currentTestResult = TEST_NULL;

bool Test::RunTests()
{
	int failedTests = 0;
	int numTests = sizeof(tests) / sizeof(testFunction);
	TIMEPOINT start = MAKE_TIMEPOINT();
	printf(ANSI_COLOR_YELLOW("[======]") " Running %d tests\n", numTests);

	Mem test_mem;
	CPU test_cpu;

	//run all tests
	for (int i = 1; i < numTests + 1; i++) {
		auto test = tests[i - 1];
		currentTestResult = Test::TEST_NULL;

		test_cpu.Reset(test_mem);
		test(test_cpu, test_mem);

		switch (currentTestResult) {
		case Test::TEST_OK:
			printf(ANSI_COLOR_GREEN("[  OK  ]") " test %d succeeded.\n", i);
			break;
		case Test::TEST_ERR:
			printf(ANSI_COLOR_RED("[FAILED]") "test %d failed.\n", i);
			failedTests++;
			break;
		case Test::TEST_NULL:
		default:
			printf(ANSI_COLOR_YELLOW("[ NULL ]") " test %d has no result.\n", i);
		}
	}

	if (failedTests != 0) {
		ASSERT(false, printf, ANSI_COLOR_RED("[FAILED]") " %d from %d tests failed.\n", failedTests, numTests);
		exit(-1);
	}
	else {
		TIMEPOINT end = MAKE_TIMEPOINT();
		printf(ANSI_COLOR_GREEN("[PASSED]") " %d tests in %lld ms.\n", numTests, (end - start).count());
	}
	return true;
}