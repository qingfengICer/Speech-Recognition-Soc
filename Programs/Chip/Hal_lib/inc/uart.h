#ifndef __USART_H
#define __USART_H

#include "soc.h"

void uart_ctrl(UART_Type *UARTx, uint32_t bitsMask, FunctionalState newState);
void uart_setBaudRate(UART_Type *UARTx, uint32_t baudRate);
FlagStatus uart_getState(UART_Type *UARTx, uint32_t bitMask);
FlagStatus uart_getITflag(UART_Type *UARTx, uint32_t bitMask);
void uart_clearITflag(UART_Type *UARTx, uint32_t bitMask);
void uart_putChar(UART_Type *UARTx, uint8_t byte);
uint8_t uart_getChar(UART_Type *UARTx);
void uart_putString(UART_Type *UARTx, char *ptr);

#endif

