// Standard Library Includes
#include <stdio.h>
#include <stdlib.h>

// Target Includes
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

// Project Includes
#include "uart.h"

// Private prototypes
void uart_rx_isr(void);

// =================================================================================

// ---------------------------------------------------------------------------------
// UART Utilities

// Set up the UART
void uart_setup(void)
{
    // Disable flow control on the UART
    uart_set_hw_flow(uart0, false, false);

    // Disable FIFOs on the UART
    uart_set_fifo_enabled(uart0, false);

    // Set up UART interrupts
    irq_set_exclusive_handler(UART0_IRQ, uart_rx_isr);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(uart0, true, false);

    // Required to prevent the first written character from being lsot
    sleep_ms(10);
}

// UART Interrupt Handler
void uart_rx_isr(void)
{
    uint8_t character = 0;

    while (uart_is_readable(uart0))
    {
        character = uart_getc(uart0);
        uart_putc(uart0, character);
    }
}