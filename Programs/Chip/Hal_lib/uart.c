#define _USART_C_
#include "uart.h"


/**
 * @brief  Config control register of uart
 *
 * @param   UARTx  pointer of UART register address mapping structure.
 *								  - UART0
 *					bitsMask	
 *					newState
 * @return	none
 *
 * 
 */
void uart_ctrl(UART_Type *UARTx, uint32_t bitsMask, FunctionalState newState)
{
	if (newState == ENABLE) {
		UARTx->CTRLR |= bitsMask;
	} else {
		UARTx->CTRLR &= ~bitsMask;
	}
}

/**
 * @brief  Config baud rate of uart
 *
 * @param   UARTx  pointer of UART register address mapping structure.
 *								  - UART0
 *					baudRate	baud rate of UART
 * @return	none
 *
 * 
 */
void uart_setBaudRate(UART_Type *UARTx, uint32_t baudRate)
{
	uint32_t tmp = PCLOCK / baudRate;
	if (tmp <= 0x10) {
		UARTx->BAUDDIV = 0x10;
	} else {
		UARTx->BAUDDIV = tmp;
	}
}

/**
 * @brief  Get state of uart
 *
 * @param   UARTx  pointer of UART register address mapping structure.
 *								  - UART0
 *					bitsMask	
 * @return	none
 *
 * 
 */
FlagStatus uart_getState(UART_Type *UARTx, uint32_t bitMask)
{
	if (UARTx->STATR & bitMask)
		return SET;
	else
		return RESET;
}

/**
 * @brief  Get interrupt state of uart
 *
 * @param   UARTx  pointer of UART register address mapping structure.
 *								  - UART0
 *					bitsMask	
 * @return	none
 *
 * 
 */
FlagStatus uart_getITflag(UART_Type *UARTx, uint32_t bitMask)
{
	if (UARTx->INTSTATR & bitMask)
		return SET;
	else
		return RESET;
}

/**
 * @brief  clear interrupt flag of uart
 *
 * @param   UARTx  pointer of UART register address mapping structure.
 *								  - UART0
 *					bitsMask	
 * @return	none
 *
 * 
 */
void uart_clearITflag(UART_Type *UARTx, uint32_t bitMask)
{
	switch(bitMask){
		case UART_INTSTATR_TX_MASK : 
		case UART_INTSTATR_RX_MASK : 
		case UART_INTSTATR_TX_MASK | UART_INTSTATR_RX_MASK : 
			UARTx->INTSTATR = bitMask;
			break;
		case UART_STATR_TX_OVERRUN_MASK : 
		case UART_STATR_RX_OVERRUN_MASK : 
		case UART_STATR_TX_OVERRUN_MASK | UART_STATR_RX_OVERRUN_MASK : 
			UARTx->STATR = bitMask;
			break;
		default : /* clear all : TX RX OverRuns */
			UARTx->INTSTATR = 0x0B;
			UARTx->STATR = 0x03;
			break;
	}
}

/**
 * @brief		Write a byte data to RX data register
 *
 * @param   UARTx  pointer of UART register address mapping structure.
 *								  - UART0
 * @return  none
 *
 * 
 */
void uart_putChar(UART_Type *UARTx, uint8_t byte)
{
	while(UARTx->STATR & UART_STATR_TX_FULL_MASK);
	UARTx->DATAR.WDATA = byte;
}

/**
 * @brief		Read a byte data from RX data register
 *
 * @param   UARTx  pointer of UART register address mapping structure.
 *								  - UART0
 * @return	none
 *
 * 
 */
uint8_t uart_getChar(UART_Type *UARTx)
{
	while((UARTx->STATR & UART_STATR_RX_FULL_MASK) == 0);
	return (uint8_t)(UARTx->DATAR.RDATA & UART_DATAR_MASK);
}

/**
 * @brief		Send a string via TX
 *
 * @param   UARTx  pointer of UART register address mapping structure.
 *								  - UART0
 *					ptr		 pointer of sting 
 * @return	none
 *
 * 
 */
void uart_putString(UART_Type *UARTx, char *ptr)
{
	while(*ptr)
		uart_putChar(UARTx, *(ptr++));
}
