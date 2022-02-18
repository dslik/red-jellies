// Include file for Calor utils

#pragma once
#ifndef UITLS_H
#define UITLS_H

// Globals
extern char binary_string[];
extern volatile uint8_t debounced_switch;


// Debug Utilities Prototypes
const char* uint8_to_binary_string(uint8_t input);
const char* uint32_to_binary_string(uint32_t input);

// WS2812 Prototypes
void ws2812_init(void);
void put_pixel(uint32_t pixel_grb);
uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);

// Temperature Sensor Prototypes
void lm75_reg_init(uint8_t address);
float lm75_reg_read(uint8_t address);

// Flash Storage Prototypes
void flash_erase_blocks(void);
uint16_t flash_find_write_offset(void);
uint16_t flash_append_value(float value);
float flash_read_value(uint16_t offset);

// Button Debouncing Prototypes
void debounce_init(void);
int64_t debounce_callback(alarm_id_t id, void *user_data);
void isr(uint gpio, uint32_t events);


#endif // UITLS_H