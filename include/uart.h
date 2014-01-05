#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init();
void uart_putc(uint8_t byte);
void uart_puts(const char* str);
uint8_t uart_getc();

#endif
