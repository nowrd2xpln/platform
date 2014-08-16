#ifndef _UART0_H
#define _UART0_H

#include "lpc2xxx.h"
#include "sysConfig.h"

/// Initializes Uart 0.
/// @note This uses Frequency variables defined at sysConfig.h
/// @param baudrate The Baud rate to set.
void init_Uart0 ( unsigned long baudrate);

/// Puts single char to UART0.
/// @param ch the char to send.
/// @returns the char that was sent.
int uart0_putchar (int ch);

/// Gets a char from UART0.
/// @returns the char captured by the UART0 Receiver.
int uart0_getchar (void);

/// Outputs a string to UART0.
/// @param string the string to send.
void uart0_putString (const char *string);

void init_Uart1 ( unsigned long baudrate);
int uart1_putchar (int ch);
int uart1_getchar (void);
void uart1_putString (const char *string);

#endif
