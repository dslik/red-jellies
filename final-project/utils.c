// Standard Library Includes
#include <stdio.h>
#include <stdlib.h>

// Target Includes
#include "hardware/pio.h"
#include "hardware/i2c.h"
#include "ws2812.pio.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

// Project Includes
#include "utils.h"

// Constants
#define I2C_SDA_PIN 16
#define I2C_SCL_PIN 17
#define WS2812_PIN  18
#define SWITCH_IN   14

#define FLASH_START XIP_BASE
#define FLASH_OFFSET (1024 * 1024)

// Globals
char binary_string[] = "00000000000000000000000000000000";

// Private Prototypes
float lm75_reg_to_degrees(uint8_t low, uint8_t high);

// =================================================================================

// ---------------------------------------------------------------------------------
// Debug Utilities

// UINT8 to binary string - Optimized logic
const char* uint8_to_binary_string(uint8_t input)
{
    uint32_t bit_count = 8;

    binary_string[bit_count] = 0;

    while(bit_count > 0)
    {
        binary_string[bit_count - 1] = '0' + ((input & (1 << (8 - bit_count))) == (1 << (8 - bit_count)));
        bit_count = bit_count - 1;
    }

    return (const char*) binary_string;
}

// UINT32 to binary string - Optimized logic
const char* uint32_to_binary_string(uint32_t input)
{
    uint32_t bit_count = 32;

    binary_string[bit_count] = 0;

    while(bit_count > 0)
    {
        binary_string[bit_count - 1] = '0' + ((input & (1 << (32 - bit_count))) == (1 << (32 - bit_count)));
        bit_count = bit_count - 1;
    }

    return (const char*) binary_string;
}

// ---------------------------------------------------------------------------------
// WS2812 Smart LED Utilities
// ---------------------------------------------------------------------------------
// Source:  Pi Pico WS2812 PIO example code
//          https://github.com/raspberrypi/pico-examples/blob/master/pio/ws2812/ws2812.c
// ---------------------------------------------------------------------------------
// License: BSD 3 Clause
// ---------------------------------------------------------------------------------

// Initialize the PIO program for talking to WS2812 LEDs
void ws2812_init(void)
{
    PIO pio = pio0;
    uint sm = pio_claim_unused_sm(pio, true);
    uint offset = pio_add_program(pio, &ws2812_program);

    pio_sm_set_enabled(pio, sm, true);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, true);
}

// Set the pixel color of a WS2812
void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Convert red, green and blue values into a uint32 value
uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

// ---------------------------------------------------------------------------------
// LM75 Temperature Sensor Utilities
// ---------------------------------------------------------------------------------
// Source: Copyright 2022 David Slik. Own work.
//         Routines based on datasheet: https://www.ti.com/lit/ds/symlink/tmp75.pdf
// ---------------------------------------------------------------------------------
// Notes:  The temperature sensor will read 0.2 - 0.5 degrees higher after approx
//         200-500 ms after being powered up.
// ---------------------------------------------------------------------------------

// Initialize the temperature sensor
void lm75_reg_init(uint8_t address)
{
    int ret;

    // Assume just powered on, so give a few ms to stablize
    sleep_ms(10);

    // Initialize the i2c subsystem
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    uint8_t txdata[2];
    
    // Set the configuration register
    txdata[0] = 0x01;
    txdata[1] = 0x60;
    ret = i2c_write_blocking (i2c_default, address, txdata, 2, false);

    // Set the pointer register
    txdata[0] = 0x00;
    ret = i2c_write_blocking (i2c_default, address, txdata, 1, false);
}

// Read the temperature value and convert it into degrees
float lm75_reg_read(uint8_t address)
{
    int ret;
    uint8_t rxdata[2];

    ret = i2c_read_blocking(i2c_default, address, (char*) &rxdata, 2, false);

    return(lm75_reg_to_degrees(rxdata[0], rxdata[1]));
}

// Convert the digital temperature output into degrees C
// See section 7.3.1 of https://www.ti.com/lit/ds/symlink/tmp75.pdf
float lm75_reg_to_degrees(uint8_t low, uint8_t high)
{
    uint16_t  temp = 0;
    float     convertedTemp = 0.0;

    temp = ((low << 8) | high) >> 4;
    convertedTemp = (float) temp * 0.0625f; 

    return(convertedTemp);
}

// ---------------------------------------------------------------------------------
// Flash Storage Utilities
// ---------------------------------------------------------------------------------
// Source: Copyright 2022 David Slik. Own work.
// ---------------------------------------------------------------------------------
// Notes:  Flash is initially set to all "1"'s. Sensor values are stored starting
// from start of flash region.
// ---------------------------------------------------------------------------------

// Erase flash blocks used for sensor value storage
// FIXME - Currently only erases a single block to minimize flash wear
void flash_erase_blocks(void)
{
    // Disable interrupts
    uint32_t interrupts = save_and_disable_interrupts();

    flash_range_erase(FLASH_OFFSET, FLASH_SECTOR_SIZE);

    // Re-enable interrupts
    restore_interrupts(interrupts);
}

// Find the next empty slot (4 byte word) that a sensor value can be written into
uint16_t flash_find_write_offset(void)
{
    uint32_t*   flash_word_accessor = NULL;
    uint16_t    counter = 0;

    flash_word_accessor = (uint32_t*) (FLASH_START + FLASH_OFFSET);

    while(flash_word_accessor[counter] != 0xFFFFFFFF)
    {
        counter = counter + 1;
    }

    return counter;
}

// Write the sensor value into the next empty slot
uint16_t flash_append_value(float value)
{
    float*      flash_accessor = NULL;
    uint32_t*   flash_word_accessor = NULL;
    uint8_t*    flash_byte_accessor = NULL;
    uint8_t     write_page_data[FLASH_PAGE_SIZE];
    uint32_t    write_page_index = 0;

    // 4 bytes per float, so 128 values per 512 byte flash page
    uint16_t    records_per_page = FLASH_PAGE_SIZE / sizeof(float);
    uint16_t    offset = flash_find_write_offset();
    uint16_t    page_num = offset / records_per_page;
    uint16_t    page_offset = offset % records_per_page;

    // Copy the current flash data into SRAM to preserve it
    flash_byte_accessor = (uint8_t*) (FLASH_START + FLASH_OFFSET + (page_num * FLASH_PAGE_SIZE));

    while(write_page_index < FLASH_PAGE_SIZE)
    {
        write_page_data[write_page_index] = flash_byte_accessor[write_page_index];

        write_page_index = write_page_index + 1;
    }

    // Check to make sure we're writing to an uninitialized flash cell
    flash_word_accessor = (uint32_t*) write_page_data;
    if(flash_word_accessor[page_offset] != 0xFFFFFFFF)
    {
        printf("ERROR - Attempting to write to offset %u that contains value 0x%X\n", offset, flash_word_accessor[page_offset]);
    }
    else
    {
        // Set the value
        flash_accessor = (float*) write_page_data;
        flash_accessor[page_offset] = value;

        flash_range_program(FLASH_OFFSET + (page_num * FLASH_PAGE_SIZE), write_page_data, FLASH_PAGE_SIZE);
    }

    return(offset);
}

// Read a stored sensor value
float flash_read_value(uint16_t offset)
{
    float* flash_accessor = NULL;

    flash_accessor = (float*) (FLASH_START + FLASH_OFFSET);

    return (float) flash_accessor[offset];
}


// ---------------------------------------------------------------------------------
// Button Deboucing
// ---------------------------------------------------------------------------------
// Source: Copyright 2022 David Slik. Own work.
// ---------------------------------------------------------------------------------
#define DEBOUNCE_DELAY  20

// Global variable for ISR
volatile uint8_t debounce_disable;
volatile uint8_t debounced_switch;

// Initialization of input lines
void debounce_init(void)
{
    gpio_init(SWITCH_IN);
    gpio_set_dir(SWITCH_IN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(SWITCH_IN,
                                       GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
                                       true, &isr);
    debounced_switch = gpio_get(SWITCH_IN);
}

// Timer callback for when button is pressed
int64_t debounce_callback(alarm_id_t id, void *user_data)
{
    uint8_t gpio_line_value;

    debounce_disable = false;
    gpio_line_value = gpio_get(SWITCH_IN);
    
    // printf("DEBUG: Debounce Callback, Switch was %i, is %i\n", debounced_switch, gpio_line_value);

    if(gpio_line_value != debounced_switch)
    {
        debounced_switch = gpio_line_value;
    }

    return false;
}

// Interrupt Service Routine
void isr(uint gpio, uint32_t events)
{
    // printf("DEBUG: events: %#x, disable %i\n", events, debounce_disable);

    if(debounce_disable == false)
    {
        debounce_disable = true;
        add_alarm_in_ms(DEBOUNCE_DELAY, &debounce_callback, NULL, false);
    }
}

