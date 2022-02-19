// Standard Library Includes
#include <stdio.h>
#include <stdlib.h>

// Target Includes
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

// Project Includes
#include "utils.h"
#include "uart.h"

#define POWER_OUT       9
#define I2C_LM75_ADDR   0x77


int main() {
    stdio_init_all();
    uart_setup();

    printf("\033[2JCalor - Starting...\n");

    // Apply 3.3v power to the LM75 temperature
    gpio_init(POWER_OUT);
    gpio_set_dir(POWER_OUT, GPIO_OUT);
    gpio_put(POWER_OUT, 1);

    // Initialize the temperature sensor
    lm75_reg_init(I2C_LM75_ADDR);

    // Initialize debounce
    debounce_init();
    
    // Initialize the WS2812 smart LED
    ws2812_init();
    put_pixel(0x00000A00);

    // Initialize the ADC
    adc_init();
    adc_gpio_init(27);
    adc_select_input(1);


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

        // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
        const float conversion_factor = 3.3f / (1 << 12);
        uint16_t result = adc_read();
        printf("Raw value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);

        sleep_ms(5000);
        put_pixel(rand());
    }
}