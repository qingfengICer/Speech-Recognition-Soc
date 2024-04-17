/**
 * @file sdram.h
 * @author shao yi (2661741248@qq.com)
 * @brief 
 * This file defines several types of SDRAM data structure and the address of each data block.
 * There are two types of audio in this project: PCM and MFCC. The PCM audio is the original audio data, and the MFCC audio is the feature vector of the audio.
 * The SDRAM is separated from the main memory.
 * You can store 4 templates and 1 test audio in the SDRAM.
 * 
 * @version 0.1
 * @date 2023-11-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _SDRAM_H_
#define _SDRAM_H_

#include <stdint.h>

typedef struct {
	int32_t *BeginPtr;	
	uint32_t DataNum ;
} SDRAM_Type_int32;

typedef struct {
	int16_t *BeginPtr;
	uint32_t DataNum ;
} SDRAM_Type_int16;

typedef struct {
	float *BeginPtr;
	uint32_t DataNum ;
} SDRAM_Type_float;

typedef struct {
	int32_t * const BeginPtr;
	uint32_t DataNum ;
} SDRAM_Type_int32_AddrFixed;

/* if there is real SDRAM */
#define SDRAM_COLUMN_SIZE						(1024*2)
#define SDRAM_ROW_SIZE							(1024*8)
#define SDRAM_BANK_SIZE							(SDRAM_ROW_SIZE * SDRAM_COLUMN_SIZE)  	/* 16MB = 0x100_0000, for each SDRAM BANK */

#define SDRAM_BASE_ADDR							0x60000000
#define SDRAM_BASE_ADDR_BANK0					(SDRAM_BASE_ADDR + SDRAM_BANK_SIZE * 0)
#define SDRAM_BASE_ADDR_BANK1					(SDRAM_BASE_ADDR + SDRAM_BANK_SIZE * 1)
#define SDRAM_BASE_ADDR_BANK2					(SDRAM_BASE_ADDR + SDRAM_BANK_SIZE * 2)
#define SDRAM_BASE_ADDR_BANK3					(SDRAM_BASE_ADDR + SDRAM_BANK_SIZE * 3)


/*************************************数据存储地址划分*****************************************************/
//#define SDRAM_MFCC_SIZE							(15600)									/* The largest MFCC has 13 d * 4 Byte * 300 frames = 15600 Byte. */
#define SDRAM_MFCC_SIZE							(25600)									/* The largest MFCC has 13 d * 4 Byte * 300 frames = 15600 Byte. */
#define SDRAM_PCM_SIZE							(1024 * 128)    						/* Sampling for 4 sec with 8kHz. 8kHz * 4s * 4Byte = , for PCM audio */

#define BASE_ADDR_AUDIO_DATA					(SDRAM_BASE_ADDR_BANK0 + SDRAM_PCM_SIZE * 0)		/* 采集的原始语音 PCM 数据 */
#define BASE_ADDR_VOICE_DATA					(SDRAM_BASE_ADDR_BANK0 + SDRAM_PCM_SIZE * 1)		/* 去除静音段的 PCM 语音数据 */

#define BASE_ADDR_MFCC_TEMPLATE					(SDRAM_BASE_ADDR_BANK0 + SDRAM_PCM_SIZE * 2)		/* Template MFCC */
#define BASE_ADDR_MFCC_TEMPLATE_01				(BASE_ADDR_MFCC_TEMPLATE + SDRAM_MFCC_SIZE * 0)		/* Template 1 */
#define BASE_ADDR_MFCC_TEMPLATE_02				(BASE_ADDR_MFCC_TEMPLATE + SDRAM_MFCC_SIZE * 1)		/* Template 2 */
#define BASE_ADDR_MFCC_TEMPLATE_03				(BASE_ADDR_MFCC_TEMPLATE + SDRAM_MFCC_SIZE * 2)		/* Template 3 */
#define BASE_ADDR_MFCC_TEMPLATE_04				(BASE_ADDR_MFCC_TEMPLATE + SDRAM_MFCC_SIZE * 3)		/* Template 4 */
#define BASE_ADDR_MFCC_TEST						(BASE_ADDR_MFCC_TEMPLATE + SDRAM_MFCC_SIZE * 4)		/* Test audio MFCC */

#endif 

