// -----------------------------------------------------------------------------
// Simple Flash Emulation Layer based on a W25Q80DVSNIG SPI Flash device
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


// -----------------------------------------------------------------------------
// Initializes the flash device by creating and erasing the backing file.
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
// Erases a single sector to all 0xFF.
// Equivilent to performing a W25Q80DVSNIG Sector Erase (20h)
// -----------------------------------------------------------------------------
void sFLASH_EraseSector(uint32_t SectorAddr)
{
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

// -----------------------------------------------------------------------------
// Erases all sectors to all 0xFF.
// Equivilent to performing a W25Q80DVSNIG Chip Erase (C7h)
// -----------------------------------------------------------------------------
void sFLASH_EraseBulk(void)
{
	uint32_t	sector_counter = 0;

	while(sector_counter != SECTORS)
	{
		sFLASH_EraseSector(sector_counter * SECTOR_SIZE);
		sector_counter = sector_counter + 1;
	}
}

// -----------------------------------------------------------------------------
// Writes data to a page. If the NumBytesToWrite goes past the end of the page,
// the data wraps around the page boundary to the start of the page.
// Equivilent to performing a W25Q80DVSNIG Page Programs (02h)
// -----------------------------------------------------------------------------
void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	assert(WriteAddr + NumByteToWrite < PAGES * PAGE_SIZE);
	assert(NumByteToWrite <= PAGE_SIZE);

	// Check if the data to write wraps around the page boundary
	if((WriteAddr / PAGE_SIZE) != ((WriteAddr + NumByteToWrite) / PAGE_SIZE))
	{
		uint32_t	page_start = (WriteAddr / PAGE_SIZE) * PAGE_SIZE;
		uint32_t	bytes_past_page = (WriteAddr + NumByteToWrite - 1) % PAGE_SIZE;

		// Write up to page boundary
		sFLASH_WriteBuffer(pBuffer, WriteAddr, NumByteToWrite - bytes_past_page - 1);		

		// Write data wrapped around page boundary
		sFLASH_WriteBuffer(&pBuffer[NumByteToWrite - bytes_past_page - 1], page_start, bytes_past_page);		
	}
	else
	{
		sFLASH_WriteBuffer(pBuffer, WriteAddr, NumByteToWrite);		
	}
}

// -----------------------------------------------------------------------------
// Writes data across one or more pages.
// Typically this would be implemented by multiple calls to sFLASH_WritePage,
// but we can just directly write to our backing file because we don't have page
// limitations like the physical device would have.
// 
// This function also simulates the situation where if you write without first
// having erased the page. Specifically, any "0"s won't be changed to "1"s.
// -----------------------------------------------------------------------------
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	assert(WriteAddr + NumByteToWrite < PAGES * PAGE_SIZE);

	uint16_t	write_counter = 0;
	char		raw_buffer = 0;

	fseek(backing_file, WriteAddr, SEEK_SET);

	while(write_counter != NumByteToWrite)
	{
		fread(&raw_buffer, 1, 1, backing_file);
		fseek(backing_file, -1, SEEK_CUR);
		raw_buffer = pBuffer[write_counter] & raw_buffer;
		fwrite(&raw_buffer, 1, 1, backing_file);
		write_counter = write_counter + 1;
	}
}

// -----------------------------------------------------------------------------
// Reads data from one or more pages.
// -----------------------------------------------------------------------------
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	assert(ReadAddr + NumByteToRead < PAGES * PAGE_SIZE);

	fseek(backing_file, ReadAddr, SEEK_SET);
	fread(pBuffer, NumByteToRead, 1, backing_file);
}



