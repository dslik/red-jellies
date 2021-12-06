// -----------------------------------------------------------------------------
// Simple Flash Emulation Layer
// Uses a backing file so you can inspect/manipulate the contents of the flash
// device using standard file tools.
// Copyright 2021 David Slik - Released into the public domain
// -----------------------------------------------------------------------------
// 2021-12-06 - Created
// -----------------------------------------------------------------------------
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>

#include "spi_flash.h"

// Global Variables
FILE* backing_file;

#define	FLASH_DATA_NAME		"flash_data"
#define	PAGE_SIZE			256
#define	PAGES				4096
#define	SECTOR_SIZE			4096
#define	SECTORS				256
#define	FLASH_ERASE_VALUE	0xFF


void sFLASH_Init(void)
{
	int file_exists = false;

	file_exists = access(FLASH_DATA_NAME, W_OK);

	if(0 != file_exists)
	{
		backing_file = fopen(FLASH_DATA_NAME, "wb+");
		sFLASH_EraseBulk();
	}
	else
	{
		backing_file = fopen(FLASH_DATA_NAME, "rb+");
	}
}

void sFLASH_EraseSector(uint32_t SectorAddr)
{
	// Equivilent to a Sector Erase (20h)
	assert(SectorAddr + SECTOR_SIZE <= PAGES * PAGE_SIZE);

	uint32_t	byte_counter = 0;
	uint8_t		init_value = FLASH_ERASE_VALUE;

	fseek(backing_file, SectorAddr, SEEK_SET);

	while(byte_counter != SECTOR_SIZE)
	{
		fwrite(&init_value, 1, 1, backing_file);
		byte_counter = byte_counter + 1;
	}
}

void sFLASH_EraseBulk(void)
{
	// Equivilent to a Chip Erase (C7h)
	uint32_t	sector_counter = 0;

	while(sector_counter != SECTORS)
	{
		sFLASH_EraseSector(sector_counter * SECTOR_SIZE);
		sector_counter = sector_counter + 1;
	}
}

void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	assert(WriteAddr + NumByteToWrite < PAGES * PAGE_SIZE);
	assert(NumByteToWrite > PAGE_SIZE);

	uint16_t	write_counter = 0;

	fseek(backing_file, WriteAddr, SEEK_SET);

	while(write_counter != NumByteToWrite)
	{
		fwrite(&pBuffer[write_counter], 1, 1, backing_file);
		write_counter = write_counter + 1;
	}
}

void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	assert(WriteAddr + NumByteToWrite < PAGES * PAGE_SIZE);

	// Normally we would write page by page, but we can cheat since we are backed by
	// a file, not an actual flash device

	uint16_t	write_counter = 0;

	fseek(backing_file, WriteAddr, SEEK_SET);

	while(write_counter != NumByteToWrite)
	{
		fwrite(&pBuffer[write_counter], 1, 1, backing_file);
		write_counter = write_counter + 1;
	}
}

void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	assert(ReadAddr + NumByteToRead < PAGES * PAGE_SIZE);

	fseek(backing_file, ReadAddr, SEEK_SET);
	fread(pBuffer, NumByteToRead, 1, backing_file);
}



