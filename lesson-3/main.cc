// -----------------------------------------------------------------------------
// Simple Flash Emulation Layer Test Runner
// Copyright 2021 David Slik - Released into the public domain
// -----------------------------------------------------------------------------
// 2021-12-06 - Created
// -----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include "spi_flash.h"
#include "test_utils.h"

// Test Runner for SPI Flash Simulator
int main(void)
{
	uint8_t		testValue = 0x00;
	char*		StringValue = "Hello Flash!";
	uint8_t		buffer[20];

	printf("SPI Flash Simulator\n");
	test_title("Testing Test Harness");
	test_run("Test Value as expected", true, true);
	test_run("Test Value not as expected", true, false);
	test_run("Test String as expected", "Hello", "Hello");
	test_run("Test String not as expected", "Hello", "hello");

	// Initialize the flash simulator
	// This creates a file that contains the flas contents
	test_title("Initilizing SPI Test Harness");
	sFLASH_Init();

	test_title("Reading Uninitialized Values");
	sFLASH_ReadBuffer(&testValue, 2048, 1);
	test_run("Uninitialized Value at 2048", 0xFF, testValue);
	sFLASH_ReadBuffer(&testValue, 4444, 1);
	test_run("Uninitialized Value at 4444", 0xFF, testValue);

	test_title("Writing Values");
	testValue = 0x00;
	sFLASH_WriteBuffer(&testValue, 2, 1);
	testValue = 0xCA;
	sFLASH_WriteBuffer(&testValue, 4, 1);

	test_title("Reading Written Values");
	sFLASH_ReadBuffer(&testValue, 2, 1);
	test_run("Uninitialized Value at 2048", 0x00, testValue);
	sFLASH_ReadBuffer(&testValue, 4, 1);
	test_run("Uninitialized Value at 4444", 0xCA, testValue);

	test_title("Writing Range of Values");
	sFLASH_WriteBuffer((uint8_t*) StringValue, 1024, strlen(StringValue) + 1);

	sFLASH_ReadBuffer(&testValue, 1024, 1);
	test_run("Uninitialized Value at start of string", 'H', testValue);

	test_title("Reading Range of Values");
	sFLASH_ReadBuffer((uint8_t*) &buffer, 1024, strlen(StringValue) + 1);
	test_run("String Value as Expected", "Hello Flash!", (char*) &buffer);

	test_title("Overwriting Values without Erase");
	testValue = 0xFE;
	sFLASH_WriteBuffer(&testValue, 6, 1);
	sFLASH_ReadBuffer(&testValue, 6, 1);
	test_run("LSB set to 0 at 6", 0xFE, testValue);
	testValue = 0xFF;
	sFLASH_WriteBuffer(&testValue, 6, 1);
	sFLASH_ReadBuffer(&testValue, 6, 1);
	test_run("LSB still set to 0 at 6", 0xFE, testValue);
	testValue = 0x00;
	sFLASH_WriteBuffer(&testValue, 6, 1);
	sFLASH_ReadBuffer(&testValue, 6, 1);
	test_run("All bits set to 0 at 6", 0x00, testValue);
	testValue = 0xFF;
	sFLASH_WriteBuffer(&testValue, 6, 1);
	sFLASH_ReadBuffer(&testValue, 6, 1);
	test_run("All bits still set to 0 at 6", 0x00, testValue);
}