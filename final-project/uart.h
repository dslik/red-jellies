// Include file for UART utils

#pragma once
#ifndef UART_H
#define UART_H

// Globals
extern volatile uint8_t collect_active;

// UART Setup
void uart_setup(void);
const char* uart_command_get(void);
void uart_command_clear(void);

#endif // UART_H