// -----------------------------------------------------------------------------
// Blink demonstration
// LED blinks when button is not held down
// Debouncing is handled via an Interrupt Service Routine
// -----------------------------------------------------------------------------
// 2021-12-17 - Created
// -----------------------------------------------------------------------------
#include <stdio.h>

#include "pico/stdlib.h"

// Pin Definitions
#define LED_PIN     8
#define POWER_OUT   9
#define SWITCH_IN   14

#define DEBOUNCE_DELAY  20

// Global variable for ISR
volatile uint8_t debounce_disable;
volatile uint8_t debounced_switch;

// -----------------------------------------------------------------------------
// Timer callback for when button is pressed or released
// This debounces the button by waiting for DEBOUNCE_DELAY then checking if
// the input level matches the edge direction.
// -----------------------------------------------------------------------------
int64_t debounce_callback(alarm_id_t id, void *user_data)
{
    uint8_t gpio_line_value;

    debounce_disable = false;
    gpio_line_value = gpio_get(SWITCH_IN);
    
    printf("DEBUG: Debounce Callback, Switch was %i, is %i\n", debounced_switch, gpio_line_value);

    if(gpio_line_value != debounced_switch)
    {
        debounced_switch = gpio_line_value;
    }

    return false;
}

// -----------------------------------------------------------------------------
// Repeating timer callback for blinking LED
// -----------------------------------------------------------------------------
bool blink_callback(struct repeating_timer *t) {
    if(debounced_switch == false)
    {
        gpio_put(LED_PIN, !gpio_get(LED_PIN));
    }

    return true;
}

// -----------------------------------------------------------------------------
// Interrupt Service Routine
// Ignores interrupts for DEBOUNCE_DELAY to wait for the switch to stabilize
// -----------------------------------------------------------------------------
void isr(uint gpio, uint32_t events)
{
    printf("DEBUG: events: %#x, disable %i\n", events, debounce_disable);

    if(debounce_disable == false)
    {
        debounce_disable = true;
        add_alarm_in_ms(DEBOUNCE_DELAY, &debounce_callback, NULL, false);
    }
}

// -----------------------------------------------------------------------------
// Main blinking program
// -----------------------------------------------------------------------------
int main(void)
{
    debounce_disable = false;
    struct repeating_timer timer;

    // Initialize UART0
    stdio_init_all();
    printf("DEBUG: Starting program\n");

    // 3.3v for our switch
    gpio_init(POWER_OUT);
    gpio_set_dir(POWER_OUT, GPIO_OUT);
    gpio_put(POWER_OUT, 1);

    // Switch input
    gpio_init(SWITCH_IN);
    gpio_set_dir(SWITCH_IN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(SWITCH_IN,
                                       GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
                                       true, &isr);
    debounced_switch = gpio_get(SWITCH_IN);

    // LED pin
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Blink Timer
    add_repeating_timer_ms(500, blink_callback, NULL, &timer);

    while(true) {
        if(debounced_switch == true)
        {
            gpio_put(LED_PIN, 1);
        }
    }
}
