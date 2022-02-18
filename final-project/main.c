// Standard Library Includes
#include <stdio.h>
#include <stdlib.h>

// Target Includes
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"

// Project Includes
#include "utils.h"

#define POWER_OUT       9
#define I2C_LM75_ADDR   0x77


int main() {
    stdio_init_all();
    printf("Calor - Starting...\n");

    // Apply 3.3v power to the LM75 temperature
    gpio_init(POWER_OUT);
    gpio_set_dir(POWER_OUT, GPIO_OUT);
    gpio_put(POWER_OUT, 1);
    
    // Give the temperature sensor time to stabilize
    sleep_ms(10);

    // Initialize the temperature sensor
    lm75_reg_init(I2C_LM75_ADDR);

    // Initialize debounce
    debounce_init();
    
    // Initialize the WS2812 smart LED
    ws2812_init();
    put_pixel(0x00000A00);

    // Sensor Acquisition Loop
    uint16_t write_offset = 0;
    float    temp;

    while (true) {
        if(debounced_switch == true)
        {
            temp = lm75_reg_read(I2C_LM75_ADDR);
            write_offset = flash_append_value(temp);
            printf("Wrote temperature %f into slot %u\n", flash_read_value(write_offset), write_offset);
        }

        sleep_ms(5000);
        put_pixel(rand());
    }
}