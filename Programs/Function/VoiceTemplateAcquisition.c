
#include "Function.h"


uint8_t WorkStage_VTA = VOICE_IDLE;
uint8_t Flag_VTA_Start = 0;
uint8_t Flag_Idle_Display_VTA = 1;
uint8_t Template_Current = 0;


int VoiceTemplateAcquisition_Main(void) 
{
	uint32_t i, j;
	uint32_t Tmp_uint32;
	int32_t AudioAverage;
	int32_t Tmp_data;
	
	VoiceSplicedData.DataNum = 0;
	MFCCSet_Testing.VectorNum = 0;
	
	WorkStage_VTA = VOICE_IDLE;
	Flag_VTA_Start = 0;
	
	printf("\r\n\t #模板采集\r\n");
	printf("\t - 采样时间: %u s\r\n", SAMPLE_TIME);
	
	while(FunctionMode == FUNCTION_MODE_TEMPLATE_ACQUISITION) {
		switch (WorkStage_VTA) {
			case VOICE_IDLE :
				if (Flag_Idle_Display_VTA) {
					/* 数码管显示工作进程: C-00 */
				
					/* 串口打印工作进程 */
					printf("\t- State : 空闲\r\n");
					Flag_Idle_Display_VTA = 0;
				}
				
				if (Flag_VTA_Start && Template_Current != 0) {
					VoiceSplicedData.DataNum = 0;
					MFCCSet_Testing.VectorNum = 0;
					WorkStage_VTA = VOICE_COLLECTION;
					Flag_VTA_Start = 0;
				}
				break;
				
			case VOICE_COLLECTION :				/* 音频采集 */
				/* 数码管显示工作进程: C-01 */
			
				/* 串口打印工作进程 */
				printf("\r\n\t- State : 语音采集\r\n");
				
				/* 开始采集 */
			  printf("\t\t- 可以开始说话了!\r\n");
				pdm2pcm_enable();
			
				/* 丢弃前10个无效数据 */
				i = 0;
				while(i < 10) {
					if (pdm2pcm_check_fifo_empty() == 0) {
						pdm2pcm_read_fifo();
						i++;
					}
				}
				
				/* 正式采集 */
				i = 0;
				AudioAverage = 0;
				while(i < AudioData.DataNum){
					if (pdm2pcm_check_fifo_empty() == 0) {
						Tmp_data = (int32_t)(pdm2pcm_read_fifo() & 0x0000ffffu);
						AudioData.BeginPtr[i++] = Tmp_data;
						AudioAverage += Tmp_data;
					}
				}
				
				/* 完成采集 */
				pdm2pcm_disable();
				
				/* 将数据转换为以0为中心 */
				AudioAverage /= (int32_t)AudioData.DataNum;
				for (i = 0; i < AudioData.DataNum; i++) {
					AudioData.BeginPtr[i] -= AudioAverage;
				}

				WorkStage_VTA = VOICE_PREPROCESSING;
				printf("\t\t- Finish!\r\n");
				break;
				
			case VOICE_PREPROCESSING :			/* 预处理 */
				/* 数码管显示工作进程: C-02 */
			
				/* 工作进程显示 */
				printf("\t- State : 音频数据处理\r\n");
				/* 语音段检测 & 预加重 */
				if (VoiceActivityDetection_Multi(AudioData.BeginPtr, AudioData.DataNum, &VoiceSegmentRecorder) != -1) {
					
					/* 语音段拼接 */
					VoiceSegmentSplicing(&VoiceSegmentRecorder, &VoiceSplicedData);
					
					/* 预加重 */
					VoicePreEmphasis(VoiceSplicedData.BeginPtr, VoiceSplicedData.DataNum);
					
					/* 标志位与状态控制变量 */
					WorkStage_VTA = VOICE_MFCC;
					printf("\t\t- Finish!\r\n");
				} else {
					printf("\t\t未检测到有效语音!\r\n");
					WorkStage_VTA = VOICE_IDLE;
					Flag_Idle_Display_VTA = 1;
				}
				break;
				
			case VOICE_MFCC :						/* MFCC特征参数提取 */
				/* 数码管显示工作进程: C-03 */
			
				/* 工作进程显示 */
				printf("\t- State : 音频Feature提取\r\n");
			
				/* 特征参数提取 */
				for (i = 0; i < (VoiceSplicedData.DataNum - FRAME_LEN + 1); i = i + FRAME_SHIFT) {
					
					/* 分帧加窗 */
					FrameCopy_HannWin(&VoiceSplicedData.BeginPtr[i], AudioGlobalData, FRAME_LEN);
					
					/* FFT */
					Transform_FFT(AudioGlobalData, FRAME_LEN);
					
					/* MFCC */
					MFFC_FeatureAbstract(AudioGlobalData, FRAME_LEN, MEL_FILTER_NUM, MFCCSet_Testing.BeginPtr[MFCCSet_Testing.VectorNum].Feature, FEATURE_DIMENSION);
					MFCCSet_Testing.VectorNum++;
				}
				
				/* 标志位与状态控制变量 */
				WorkStage_VTA = VOICE_MFCC_STORE;
				printf("\t\t- Finish!\r\n");
				break;
				
				case VOICE_MFCC_STORE :		/* 模板存储 */
					/* 数码管显示工作进程: C-04 */
			
					/* 工作进程显示 */
					printf("\t- State : 音频Feature存储\r\n");
				
					/* 模板存储 */
					switch (Template_Current) {
						case 1 :
							Tmp_uint32 = MFCCSet_Testing.VectorNum;
							TemplateMatchArray[0].Ptr_VectorSet->VectorNum  = Tmp_uint32;
							for (i = 0; i < Tmp_uint32; i++) { /* 特征向量数量 */
								for (j = 0; j < FEATURE_DIMENSION; j++) {  /* 一个特征向量中特征值个数 */
									TemplateMatchArray[0].Ptr_VectorSet->BeginPtr[i].Feature[j] = MFCCSet_Testing.BeginPtr[i].Feature[j];
								}
							}
							break;
							
						case 2 :
							Tmp_uint32 = MFCCSet_Testing.VectorNum;
							TemplateMatchArray[1].Ptr_VectorSet->VectorNum  = Tmp_uint32;
							for (i = 0; i < Tmp_uint32; i++) { /* 特征向量数量 */
								for (j = 0; j < FEATURE_DIMENSION; j++) {  /* 一个特征向量中特征值个数 */
									TemplateMatchArray[1].Ptr_VectorSet->BeginPtr[i].Feature[j] = MFCCSet_Testing.BeginPtr[i].Feature[j];
								}
							}
							break;
							
						case 3 :
							Tmp_uint32 = MFCCSet_Testing.VectorNum;
							TemplateMatchArray[2].Ptr_VectorSet->VectorNum  = Tmp_uint32;
							for (i = 0; i < Tmp_uint32; i++) { /* 特征向量数量 */
								for (j = 0; j < FEATURE_DIMENSION; j++) {  /* 一个特征向量中特征值个数 */
									TemplateMatchArray[2].Ptr_VectorSet->BeginPtr[i].Feature[j] = MFCCSet_Testing.BeginPtr[i].Feature[j];
								}
							}
							break;
							
						case 4 :
							Tmp_uint32 = MFCCSet_Testing.VectorNum;
							TemplateMatchArray[3].Ptr_VectorSet->VectorNum  = Tmp_uint32;
							for (i = 0; i < Tmp_uint32; i++) { /* 特征向量数量 */
								for (j = 0; j < FEATURE_DIMENSION; j++) {  /* 一个特征向量中特征值个数 */
									TemplateMatchArray[3].Ptr_VectorSet->BeginPtr[i].Feature[j] = MFCCSet_Testing.BeginPtr[i].Feature[j];
								}
							}
							break;
							
						case 0xff :
							printf("\t\t- 模板结果展示 : %u\r\n", MFCCSet_Testing.VectorNum);
							Tmp_uint32 = MFCCSet_Testing.VectorNum;
							for (i = 0; i < Tmp_uint32; i++) { /* 特征向量数量 */
								printf("{ ");
								for (j = 0; j < FEATURE_DIMENSION; j++) {  /* 一个特征向量中特征值个数 */
									if (j == FEATURE_DIMENSION - 1) {
										printf("%f  ", MFCCSet_Testing.BeginPtr[i].Feature[j]);
									} else {
										printf("%f,  ", MFCCSet_Testing.BeginPtr[i].Feature[j]);
									}
								}
								if (i == Tmp_uint32 - 1) {
									printf("}\r\n");
								} else {
									printf("},\r\n");
								}
							}
							break;
					}
					/* 标志位与状态控制变量 */
				WorkStage_VTA = VOICE_IDLE;
				Flag_Idle_Display_VTA = 1;
				printf("\t\t- Finish!\r\n");
				break;
		} 
	}
	
	return 0;
}

