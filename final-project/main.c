// Standard Library Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    printf("\033[2JCalor - Starting up...\n");

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


    printf("Calor - Ready\n");

    // Sensor Acquisition Loop
    uint16_t write_offset = 0;
    float    temp;

    while (true) {
        // Check if there are any commands pending
        const char* command = uart_command_get();
        if(strcmp(command, "") != 0)
        {
            if(strcmp(command, "help") == 0)
            {
                printf("Commands:\n");
                printf("\"voltage\"            - Displays the current VCC rail voltage\n");
                printf("\"temp\"               - Displays the current temperature\n");
                printf("\"temp records count\" - Displays the current temperature\n");
                printf("\"temp records print\" - Displays the current temperature\n");
                printf("\"temp records clear\" - Displays the current temperature\n");
                printf("\"colour\"             - Changes the color of the RGB LED to a random value\n");
                printf("\"clear\"              - Clear the screen\n");
                printf("CTRL-C                 - Escape from command mode\n");
                uart_command_clear();
            }
            else if(strcmp(command, "voltage") == 0)
            {
                // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
                const float conversion_factor = 3.3f / (1 << 12);
                uint16_t result = adc_read();
                printf("Current voltage is: %f V (Raw value: 0x%03x)\n", result * conversion_factor, result);
                uart_command_clear();
            }
            else if(strcmp(command, "temp") == 0)
            {
                temp = lm75_reg_read(I2C_LM75_ADDR);
                printf("Current temperature is: %f %cC\n", temp, 0xB0);
                uart_command_clear();
            }
            else if(strcmp(command, "temp records count") == 0)
            {
                printf("There are %u records stored.\n", flash_find_write_offset());
                uart_command_clear();
            }
            else if(strcmp(command, "temp records print") == 0)
            {
                uint16_t num_records = flash_find_write_offset();
                uint16_t counter = 0;

                printf("{ \"temperatures\" : [");
                while(counter < num_records)
                {
                    printf(" %f", flash_read_value(counter));

                    if(counter + 1 != num_records)
                    {
                        printf(",");
                    }

                    counter = counter + 1;
                }
                printf(" ] }\n");
                uart_command_clear();
            }
            else if(strcmp(command, "temp records clear") == 0)
            {
                printf("Erasing Flash...\n");
                flash_erase_blocks();
                printf("Flash erased.\n");
                uart_command_clear();
            }
            else if(strcmp(command, "colour") == 0)
            {
                put_pixel(rand());
                uart_command_clear();
            }
            else if(strcmp(command, "clear") == 0)
            {
                printf("\033[2J");
                uart_command_clear();
            }
            else
            {
                printf("Unknown command '%s'\n", command);
                uart_command_clear();
            }
        }
        else
        {
            // If a command is not being collected
            if(collect_active == 0)
            {
                if(debounced_switch == true)
                {
                    temp = lm75_reg_read(I2C_LM75_ADDR);
                    write_offset = flash_append_value(temp);
                    printf("Wrote temperature %f into slot %u\n", flash_read_value(write_offset), write_offset);
                }

                //sleep_ms(5000);
            }
        }

        sleep_ms(100);
    }
}