
#include "Function.h"

uint8_t FunctionMode = FUNCTION_MODE_IDEL;

SDRAM_Type_int32 AudioData = {(int32_t *)BASE_ADDR_AUDIO_DATA, SAMPLE_TIME * SAMPLE_FREQ};				
SDRAM_Type_int32 VoiceSplicedData = {(int32_t *)BASE_ADDR_VOICE_DATA, 0}; 										/* 地址固定，程序中不要修改 */
VoiceSegmentRecorder_Type VoiceSegmentRecorder ;
Complex_Type AudioGlobalData[FRAME_LEN] = {0};

DTW_VectorSet_Type MFCCSet_Testing = {(DTW_FeatureVector_Type *)BASE_ADDR_MFCC_TEST, 0}; 		/* 地址固定，程序中不要修改 */

DTW_VectorSet_Type MFCCSet_Template_01 = {(DTW_FeatureVector_Type *)BASE_ADDR_MFCC_TEMPLATE_01, 0}; 		/* 地址固定，程序中不要修改 */
DTW_VectorSet_Type MFCCSet_Template_02 = {(DTW_FeatureVector_Type *)BASE_ADDR_MFCC_TEMPLATE_02, 0}; 		/* 地址固定，程序中不要修改 */
DTW_VectorSet_Type MFCCSet_Template_03 = {(DTW_FeatureVector_Type *)BASE_ADDR_MFCC_TEMPLATE_03, 0}; 		/* 地址固定，程序中不要修改 */
DTW_VectorSet_Type MFCCSet_Template_04 = {(DTW_FeatureVector_Type *)BASE_ADDR_MFCC_TEMPLATE_04, 0}; 		/* 地址固定，程序中不要修改 */

TemplateMatch_Type TemplateMatchArray[TEMPLATE_NUM] = {
		{&MFCCSet_Template_01, 0},
		{&MFCCSet_Template_02, 0},
		{&MFCCSet_Template_03, 0},
		{&MFCCSet_Template_04, 0}
};



int main(void)
{

	NVIC_SetPriorityGrouping(0);
	debug_init();
	SysTick_init();
	CreateSineTable();
	MFCC_SetFilterTable();
	
	keyboard_init();
	
	pdm2pcm_init();
	
	printf("\t\t Cortex-M3 start up!\r\n");
	printf("=========================使用说明========================\r\n");
	printf("\t -指令AA —— 模板加载（离线）\r\n");
	printf("\t\t -4个默认模板：\r\n");
	printf("\t\t\t -模板1：Lv Yu Xuan\r\n");
	printf("\t\t\t -模板2: Liu Rui\r\n");
	printf("\t\t\t -模板3: Shao Yi\r\n");
//	printf("\t\t\t -模板4：Ji Chen Dian Lu\r\n");
	printf("\t\t\t -模板4：An Lu Ke Ji\r\n");
	printf("\t -指令BB —— 模板录入（在线）\r\n");
	printf("\t\t -指令B1-B4 —— 分别录入4个模板\r\n");
	printf("\t\t -指令BF —— 输出模板Feature\r\n");
	printf("\t -指令CC —— 语音识别\r\n");
	printf("\t\t -指令C1 —— 开始录音\r\n");
	printf("============================END==========================\r\n");
	
	int load_flag = 0;
	
	while(1) {
		switch (FunctionMode) {
			case FUNCTION_MODE_IDEL :
				
				break;
			case FUNCTION_MODE_TEST : 
				Test_Main();
				break;
			case FUNCTION_MODE_VOICE_RECONITION : 
				VoiceRecognition_Main(load_flag);
				break;
			case FUNCTION_MODE_TEMPLATE_ACQUISITION : 
				load_flag = VoiceTemplateAcquisition_Main();
				break;
			case FUNCTION_MODE_TEMPLATE_LOAD : 
				load_flag = VoiceTemplateLoad_Main();
				//FunctionMode = FUNCTION_MODE_VOICE_RECONITION;
			FunctionMode = FUNCTION_MODE_IDEL;
				break;
		}
	}
}

void KEY_Handler(void)
{
//		printf("111");
//	
//	uint32_t din;
//	//din = KeyboardReg;
//	
//	din = 0x00000004;

//	printf("111");
//	
//	if(din == 0x00000004)					// S3
//	{
//					printf("222");
//	}
//	else if(din & 0x00000001)			// S1
//	{
//					//printf("333");
//  }
}
