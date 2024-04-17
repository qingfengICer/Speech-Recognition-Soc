
#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "hal_lib.h"
#include "debug.h"
#include "delay.h"
#include "fft.h"
#include "mfcc.h"
#include "sdram.h"
#include "VoicePreprocessing.h"
#include "dtw.h"
#include "asio.h"
#include "Keyboard.h"

typedef struct {
	DTW_VectorSet_Type *Ptr_VectorSet;
	float dtw_result;
} TemplateMatch_Type;

#define TEMPLATE_NUM		4
extern TemplateMatch_Type TemplateMatchArray[TEMPLATE_NUM];

#define SAMPLE_FREQ			8000
#define SAMPLE_TIME			4
#define SAMPLE_DEPTH		16
#define FRAME_LEN				256
#define FRAME_SHIFT			128
#define FFT_N						256

#define FUNCTION_MODE_IDEL										0x00
#define FUNCTION_MODE_TEST										0x01
#define FUNCTION_MODE_VOICE_RECONITION				0x02
#define FUNCTION_MODE_TEMPLATE_ACQUISITION		0x03
#define FUNCTION_MODE_TEMPLATE_LOAD						0x04
extern uint8_t FunctionMode;

extern SDRAM_Type_int32 AudioData ;
extern VoiceSegmentRecorder_Type VoiceSegmentRecorder ;
extern SDRAM_Type_int32 VoiceSplicedData; 	/* 地址固定，程序中不要修改 */
extern Complex_Type AudioGlobalData[FRAME_LEN];
extern DTW_VectorSet_Type MFCCSet_Testing;
extern TemplateMatch_Type TemplateMatchArray[TEMPLATE_NUM];


/****************************** Function: Test ****************************/
#define TEST_MODE_IDLE				0x00
#define TEST_MODE_UART				0x01
#define TEST_MODE_SDRAM				0x02

int Test_Main(void);

/****************************** Function: Voice Recognition ****************************/
//#define VOICE_RECOGNITION_STEP_VERIFY

#define VOICE_IDLE											0x00
#define VOICE_COLLECTION								0x01
#define VOICE_PREPROCESSING							0x02
#define VOICE_MFCC											0x03
#define VOICE_DTW												0x04
#define VOICE_RESUALT										0x05
#define VOICE_MFCC_STORE								0x06

#define DTW_COEFF_AMP										100
#define DTW_THRESHOLD										2000

extern uint8_t WorkStage_VR;
extern uint8_t Flag_VR_Start ;
extern uint8_t Flag_Idle_Display_VR;
extern uint8_t Flag_VR_StepVerify;

int32_t VoiceRecognition_Main(int load_flag);
/****************************** Function: Voice Template Acquisition ****************************/

extern uint8_t WorkStage_VTA;
extern uint8_t Flag_VTA_Start;
extern uint8_t Flag_Idle_Display_VTA;
extern uint8_t Template_Current;
int VoiceTemplateAcquisition_Main(void);

/****************************** Function: Voice Template Load ****************************/
int VoiceTemplateLoad_Main(void);

#endif

