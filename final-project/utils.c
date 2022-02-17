#include <stdio.h>
#include <stdlib.h>

#include "hardware/pio.h"
#include "hardware/i2c.h"
#include "ws2812.pio.h"

#include "utils.h"

#define I2C_SDA_PIN 16
#define I2C_SCL_PIN 17
#define WS2812_PIN  18


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
// From Pi Pico WS2812 PIO Example

void ws2812_init(void)
{
    PIO pio = pio0;
    uint sm = pio_claim_unused_sm(pio, true);
    uint offset = pio_add_program(pio, &ws2812_program);

    pio_sm_set_enabled(pio, sm, true);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, true);
}

void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

// ---------------------------------------------------------------------------------
// LM75 Temperature Sensor Utilities

// Initialize the temperature sensor
void lm75_reg_init(uint8_t address)
{
    int ret;

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
