// -----------------------------------------------------------------------------
// Test Runner Utility Functions
// Copyright 2021 David Slik - Released into the public domain
// -----------------------------------------------------------------------------
// 2021-12-06 - Created
// -----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include "test_utils.h"

void test_title(const char* title)
{
	printf("-----------------------------------------------------------------------------");
	printf("\x1B[0G");
	printf("\x1B[1;97m%s\x1B[0m \n", title);
}

void test_run(const char* title, uint64_t expected_value, uint64_t actual_value)
{
	printf("%s {E: %llu, A: %llu}", title, expected_value, actual_value);
	if(expected_value == actual_value)
	{
		printf("\x1B[71G [\x1B[1;32mPASS\x1B[0m]\n");
	}
	else
	{
		printf("\x1B[71G [\x1B[31mFAIL\x1B[0m]\n");
	}
}

void test_run(const char* title, char* expected_value, char* actual_value)
{
	printf("%s {E: %s, A: %s}", title, expected_value, actual_value);
	if(0 == strcmp(expected_value, actual_value))
	{
		printf("\x1B[71G [\x1B[1;32mPASS\x1B[0m]\n");
	}
	else
	{
		printf("\x1B[71G [\x1B[31mFAIL\x1B[0m]\n");
	}
}