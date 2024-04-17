
#define _DEBUG_C_

#include <stdio.h>
#include "debug.h"
#include "delay.h"
#include "hal_lib.h"
#include "sdram.h"
#include "Function.h"

/**
 * @brief		Initialize UART periferal
 *
 * @param   UARTx  pointer of UART register address mapping structure.
 *								  - UART0
 *					ptr		 pointer of sting 
 * @return	none
 *
 * 
 */
void debug_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	uart_setBaudRate(UART0, DEBUG_BAUDRATE);
	uart_ctrl(UART0, UART_CTRLR_TX_EN_MASK | UART_CTRLR_RX_EN_MASK , ENABLE);
	uart_ctrl(UART0, UART_CTRLR_RX_INT_MASK | UART_CTRLR_RX_ORINT_MASK , ENABLE);
	
	/* enable RX interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = UART0_TXRX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* enable over run interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = UART0_Over_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void keyboard_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* enable keyboard interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = KEYBOARD_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


/* ######################   printf() redirect   ############################ */
#ifdef USE_MCROLIB_MDK
/**
 * @brief		redefine the function "fputc(int ch, FILE *stream)" to support
 *					function "printf"
 *
 * @param   ch  
 *					stream		 
 * @return	none
 *
 * 
 */
int fputc(int ch, FILE *stream)
{
	while(UART0->STATR & UART_STATR_TX_FULL_MASK);
	UART0->DATAR.WDATA = (uint8_t)ch;
	
	return ch;
}

#endif

#ifdef USE_STDLIB_MDK

/* inform linker do not use semihosting function from C library */
#pragma import(__use_no_semihosting)
/* define _sys_exit() to avoid using semihosting mode */
void _sys_exit(int x)
{
	x = x;
}
/* define required data type in standard library */
struct __FILE {
	int handle;
};

FILE __stdout;

/**
 * @brief		redefine the function "fputc(int ch, FILE *stream)" to support
 *					function "printf"
 *
 * @param   ch  
 *					stream		 
 * @return	none
 *
 * 
 */
int fputc(int ch, FILE *stream)
{
	while(UART0->STATR & UART_STATR_TX_FULL_MASK);
	UART0->DATAR.WDATA = (uint8_t)ch;
	
	return ch;
}


#endif

/* ######################   UART receive   ############################ */
/**
 * @brief		USART0 TX/RX interrupt service routine
 *
 * @param   none
 * @return	none
 *
 * 
 */
void UART0_TXRX_Handler(void)
{
	
	if ( UART0->INTSTATR & UART_INTSTATR_RX_MASK) { 
		/* RX interrupt service code */
		switch (UART0->DATAR.RDATA & 0xff) {
			/* 功能选择与切换指令 0xF- */
			case 0xf0 : /* 空闲功能 */
				FunctionMode = FUNCTION_MODE_IDEL;
				break;
			case 0xaa : /* 模板加载功能 */
				FunctionMode = FUNCTION_MODE_TEMPLATE_LOAD;
				break;
			case 0xbb : /* 模板采集功能 */
				FunctionMode = FUNCTION_MODE_TEMPLATE_ACQUISITION;
				break;
			case 0xcc : /* 语音识别功能 */
				FunctionMode = FUNCTION_MODE_VOICE_RECONITION;
				break;
			case 0xff : /* 测试功能 */
				FunctionMode = FUNCTION_MODE_TEST;
				break;
			
			/* 语音识别相关指令 0xA-*/
			case 0xc0 : /* 空闲状态 */
				WorkStage_VR = VOICE_IDLE;
				Flag_VR_Start = 0;
				Flag_Idle_Display_VR = 1;
				break;
			case 0xc1 : /* 开始采集语音 */
				Flag_VR_Start = 1;
				break;
			case 0xcf : /* 验证控制-单步 */
				#ifdef VOICE_RECOGNITION_STEP_VERIFY
					Flag_VR_StepVerify = 0;
				#endif
				break;
			
			/* 模板采集相关指令相关指令 0xC- */
			case 0xb0 : /* 空闲状态 */
				WorkStage_VTA = VOICE_IDLE;
				Flag_VTA_Start = 0;
				Flag_Idle_Display_VTA = 1;
				Template_Current = 0;
				break;
			case 0xb1 : /* 录入模板01 */
				Flag_VTA_Start = 1;
				Template_Current = 1;
				break;
			case 0xb2 : /* 录入模板02 */
				Flag_VTA_Start = 1;
				Template_Current = 2;
				break;
			case 0xb3 : /* 录入模板03 */
				Flag_VTA_Start = 1;
				Template_Current = 3;
				break;
			case 0xb4 : /* 录入模板04 */
				Flag_VTA_Start = 1;
				Template_Current = 4;
				break;
			case 0xbf : /* 模板输出 */
				Flag_VTA_Start = 1;
				Template_Current = 0xff;
				break;
			
			/* 测试相关指令 0x0- */
			case 0x00 :
				
				break;
			
			/* 模板加载相关指令 */
			
		}
	}
	/* clear interrupts (write 1 to clear) */
	UART0->INTSTATR = UART_INTSTATR_RX_MASK | UART_INTSTATR_TX_MASK;
}

/**
 * @brief		USART0 TX/RX over run interrupt service routine
 *
 * @param   none
 * @return	none
 *
 * 
 */
void UART0_Over_Handler(void)
{
	printf("uart over run!\r\n");
	/* clear interrupts (write 1 to clear) */
	UART0->STATR = UART_STATR_RX_OVERRUN_MASK | UART_STATR_TX_OVERRUN_MASK;
}


//void KEY_Handler(void)
//{
//	
//	uint32_t din;
//	din = KeyboardReg;
//	
//	if(din & 0x00000001)					// S1，模板加载功能
//	{
//					FunctionMode = FUNCTION_MODE_TEMPLATE_LOAD;
//	}
//	else if(din & 0x00000002)			// S2，模板采集功能
//	{
//					FunctionMode = FUNCTION_MODE_TEMPLATE_ACQUISITION;
//  }
//	else if(din & 0x00000004)			// S3，语音识别功能
//	{
//					FunctionMode = FUNCTION_MODE_VOICE_RECONITION;
//  }
//	else if(din & 0x00000008)			// S4，测试功能
//	{
//					FunctionMode = FUNCTION_MODE_TEST;
//  }
//	else if(din & 0x00000010)			// S5，录入模板1
//	{
//				Flag_VTA_Start = 1;
//				Template_Current = 1;
//  }
//	else if(din & 0x00000020)			// S6，录入模板2
//	{
//				Flag_VTA_Start = 1;
//				Template_Current = 2;	
//  }
//	else if(din & 0x00000040)			// S7，录入模板3
//	{
//				Flag_VTA_Start = 1;
//				Template_Current = 3;
//  }
//	else if(din & 0x00000080)			// S8，录入模板4
//	{
//				Flag_VTA_Start = 1;
//				Template_Current = 4;	
//  }
//}
