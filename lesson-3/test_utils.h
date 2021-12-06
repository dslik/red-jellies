// -----------------------------------------------------------------------------
// Test Runner Utility Function Prototypes
// Copyright 2021 David Slik - Released into the public domain
// -----------------------------------------------------------------------------
// 2021-12-06 - Created
// -----------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>

void test_title(const char* title);
void test_run(const char* title, uint64_t expected_value, uint64_t actual_value);
void test_run(const char* title, char* expected_value, char* actual_value);

