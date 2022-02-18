// Include file for Calor utils

#pragma once
#ifndef UITLS_H
#define UITLS_H

// Globals
extern char binary_string[];

// Prototypes
void ws2812_init(void);
void put_pixel(uint32_t pixel_grb);
uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);

const char* uint8_to_binary_string(uint8_t input);
const char* uint32_to_binary_string(uint32_t input);

void lm75_reg_init(uint8_t address);
float lm75_reg_read(uint8_t address);

void flash_erase_blocks(void);
uint16_t flash_find_write_offset(void);
uint16_t flash_append_value(float value);
float flash_read_value(uint16_t offset);

#endif // UITLS_H