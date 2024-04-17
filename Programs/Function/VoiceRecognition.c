
#include "Function.h"

uint8_t WorkStage_VR = VOICE_IDLE;
uint8_t Flag_VR_Start = 0;
uint8_t Flag_Idle_Display_VR = 1;

#ifdef VOICE_RECOGNITION_STEP_VERIFY
	uint8_t Flag_VR_StepVerify = 1;
#endif

int32_t VoiceRecognition_Main(int load_flag) 
{
	uint32_t i;
//	uint32_t u,v;
	float Tmp_float;
	uint32_t Tmp_uint32;
	int32_t AudioAverage;
	int32_t Tmp_data;
	
	VoiceSplicedData.DataNum = 0;
	MFCCSet_Testing.VectorNum = 0;
	
	WorkStage_VR = VOICE_IDLE;
	Flag_Idle_Display_VR = 1;
	Flag_VR_Start = 0;
	
	printf("\r\n\t #����ʶ�� \r\n");
	
	#ifdef VOICE_RECOGNITION_STEP_VERIFY
		printf("============Step Verify Mode============\r\n");
		Flag_VR_StepVerify = 1;
	#endif
	
	//printf("\t - ����Ƶ��: %u Hz\r\n", SAMPLE_FREQ);
	printf("\t - ����ʱ��: %u s\r\n", SAMPLE_TIME);
	//printf("\t - �������: %u bit\r\n", SAMPLE_DEPTH);
	printf("\t - ģ������: %u\r\n", TEMPLATE_NUM);
	
	while (FunctionMode == FUNCTION_MODE_VOICE_RECONITION) {
		switch (WorkStage_VR) {
			case VOICE_IDLE :
				if (Flag_Idle_Display_VR) {
					/* �������ʾ��������: A-00 */
					ASIO_Tube_Set(5, 0xC, POINT_OFF);
					ASIO_Tube_Set(4, 0x0, POINT_OFF);
					/* ���ڴ�ӡ�������� */
					printf("\t- State : ����\r\n");
					Flag_Idle_Display_VR = 0;
				}
				
				if (Flag_VR_Start) {
					VoiceSplicedData.DataNum = 0;
					MFCCSet_Testing.VectorNum = 0;
					WorkStage_VR = VOICE_COLLECTION;
					Flag_VR_Start = 0;
				}
				break;
				
			case VOICE_COLLECTION :				/* ��Ƶ�ɼ� */
				/* �������ʾ��������: A-01 */
				ASIO_Tube_Set(5, 0xC, POINT_OFF);
				ASIO_Tube_Set(4, 0x1, POINT_OFF);
				/* ���ڴ�ӡ�������� */
				printf("\r\n\t- State : �����ɼ�\r\n");
				
				/* ��ʼ�ɼ� */
				printf("\t\t- ���Կ�ʼ˵����!\r\n");
				pdm2pcm_enable();
				
				/* ����ǰ10����Ч���� */
				i = 0;
				while(i < 10) {
					if (pdm2pcm_check_fifo_empty() == 0) {
						pdm2pcm_read_fifo();
						i++;
					}
				}
				
				/* ��ʽ�ɼ� */
				i = 0;
				AudioAverage = 0;
				while(i < AudioData.DataNum){
					if (pdm2pcm_check_fifo_empty() == 0) {
						Tmp_data = (int32_t)(pdm2pcm_read_fifo() & 0x0000ffffu);
						AudioData.BeginPtr[i++] = Tmp_data;
						AudioAverage += Tmp_data;
					}
				}
				
				/* ��ɲɼ� */
				pdm2pcm_disable();
				
				#ifdef VOICE_RECOGNITION_STEP_VERIFY  /* ������֤�����ԭʼ�������� */
					printf("Original Audio Data:\r\n");
					for (i = 0; i < AudioData.DataNum; i++) {
						printf("%d ", AudioData.BeginPtr[i]);
					}
					printf("\r\n\r\n");
				#endif
				
				/* ������ת��Ϊ��0Ϊ���� */
				AudioAverage /= (int32_t)AudioData.DataNum;
				for (i = 0; i < AudioData.DataNum; i++) {
					Tmp_data = AudioData.BeginPtr[i] - AudioAverage;
					AudioData.BeginPtr[i] = Tmp_data;
				}
				
				#ifdef VOICE_RECOGNITION_STEP_VERIFY  /* ������֤�����ת������0Ϊ���ĵ����� */
				printf("Zero-centered Audio Data - average: %d\r\n", AudioAverage);
					for (i = 0; i < AudioData.DataNum; i++) {
						printf("%d ", AudioData.BeginPtr[i]);
					}
					printf("\r\n\r\n");
				#endif
				
				/* ��־λ��״̬���Ʊ��� */
				WorkStage_VR = VOICE_PREPROCESSING;
				printf("\t\t- Finish!\r\n");
				
				/* ��֤����-�������� */
				#ifdef VOICE_RECOGNITION_STEP_VERIFY
					while(Flag_VR_StepVerify);
					Flag_VR_StepVerify = 1;
				#endif
				break;
				
			case VOICE_PREPROCESSING :			/* Ԥ���� */
				/* �������ʾ��������: A-02 */
				ASIO_Tube_Set(5, 0xC, POINT_OFF);
				ASIO_Tube_Set(4, 0x2, POINT_OFF);
				/* ����������ʾ */
				printf("\t- State : ��Ƶ���ݴ���\r\n");
				/* �����μ�� & Ԥ���� */
				if (VoiceActivityDetection_Multi(AudioData.BeginPtr, AudioData.DataNum, &VoiceSegmentRecorder) != -1) {
					
					/* ������ƴ�� */
					VoiceSegmentSplicing(&VoiceSegmentRecorder, &VoiceSplicedData);
					
					#ifdef VOICE_RECOGNITION_STEP_VERIFY  /* ������֤�����ȥ�������ε����� */
						printf("Remove Silent Segment Voice Data:\r\n");
						for (i = 0; i < VoiceSplicedData.DataNum; i++) {
							printf("%d ", VoiceSplicedData.BeginPtr[i]);
						}
						printf("\r\n\r\n");
					#endif
					
					/* Ԥ���� */
					VoicePreEmphasis(VoiceSplicedData.BeginPtr, VoiceSplicedData.DataNum);
					
					#ifdef VOICE_RECOGNITION_STEP_VERIFY  /* ������֤�����Ԥ���غ������ */
						printf("Pre-weight Voice Data:\r\n");
						for (i = 0; i < VoiceSplicedData.DataNum; i++) {
							printf("%d ", VoiceSplicedData.BeginPtr[i]);
						}
						printf("\r\n\r\n");
					#endif
					
					/* ��־λ��״̬���Ʊ��� */
					WorkStage_VR = VOICE_MFCC;
					printf("\t\t- Finish!\r\n");
				} else {
					printf("\t\t- δ��⵽��Ч����!\r\n");
					WorkStage_VR = VOICE_IDLE;
					Flag_Idle_Display_VR = 1;
				}
				
				/* ��֤����-�������� */
				#ifdef VOICE_RECOGNITION_STEP_VERIFY
					while(Flag_VR_StepVerify);
					Flag_VR_StepVerify = 1;
				#endif
				break;
				
			case VOICE_MFCC :						/* MFCC����������ȡ */
				/* �������ʾ��������: A-03 */
				ASIO_Tube_Set(5, 0xC, POINT_OFF);
				ASIO_Tube_Set(4, 0x3, POINT_OFF);
				/* ����������ʾ */
				printf("\t- State : ��ƵFeature��ȡ\r\n");
			
				/* ����������ȡ */
				for (i = 0; i < (VoiceSplicedData.DataNum - FRAME_LEN + 1); i = i + FRAME_SHIFT) {
					
					/* ��֡�Ӵ� */
					FrameCopy_HannWin(&VoiceSplicedData.BeginPtr[i], AudioGlobalData, FRAME_LEN);
					
					/* FFT */
					Transform_FFT(AudioGlobalData, FRAME_LEN);
					
					/* MFCC */
					MFFC_FeatureAbstract(AudioGlobalData, FRAME_LEN, MEL_FILTER_NUM, MFCCSet_Testing.BeginPtr[MFCCSet_Testing.VectorNum].Feature, FEATURE_DIMENSION);
					MFCCSet_Testing.VectorNum++;
				}
				
				#ifdef VOICE_RECOGNITION_STEP_VERIFY  /* ������֤�����MFCC���������� */
					printf("MFCC Feature Vector Set:\r\n");
					for (i = 0; i < MFCCSet_Testing.VectorNum; i++) {
						for (j = 0; j < FEATURE_DIMENSION; j++) {
							printf("%f ", MFCCSet_Testing.BeginPtr[i].Feature[j]);
						}
						printf("\r\n");
					}
					printf("\r\n\r\n");
				#endif
				
				/* ��־λ��״̬���Ʊ��� */
				WorkStage_VR = VOICE_DTW;
				printf("\t\t- Finish!\r\n");
				
				/* ��֤����-�������� */
				#ifdef VOICE_RECOGNITION_STEP_VERIFY
					while(Flag_VR_StepVerify);
					Flag_VR_StepVerify = 1;
				#endif
				break;
			
			/* DTW */
			case VOICE_DTW :						/* �����ȶ� */
				/* �������ʾ��������: A-04 */
				ASIO_Tube_Set(5, 0xC, POINT_OFF);
				ASIO_Tube_Set(4, 0x4, POINT_OFF);
				/* ����������ʾ */
				printf("\t- State : �����ȶ�\r\n");
			
				/* �����ȶ� */
				for (i = 0; i < TEMPLATE_NUM; i++) {
					if (TemplateMatchArray[i].Ptr_VectorSet->VectorNum == 0) {
						printf("\t\t- ģ�嶪ʧ - %u\r\n", i);
						return -1;
					}
				//TEST
//				printf("\t\t- ģ����չʾ : %u\r\n", MFCCSet_Testing.VectorNum);
//							Tmp_uint32 = MFCCSet_Testing.VectorNum;
//							for (u = 0; u < Tmp_uint32; u++) { /* ������������ */
//								printf("{ ");
//								for (v = 0; v < FEATURE_DIMENSION; v++) {  /* һ����������������ֵ���� */
//									if (v == FEATURE_DIMENSION - 1) {
//										printf("%f  ", MFCCSet_Testing.BeginPtr[u].Feature[v]);
//									} else {
//										printf("%f,  ", MFCCSet_Testing.BeginPtr[u].Feature[v]);
//									}
//								}
//								if (u == Tmp_uint32 - 1) {
//									printf("}\r\n");
//								} else {
//									printf("},\r\n");
//								}	
//					}
//				printf("\t\t- ģ����չʾ : %u\r\n", TemplateMatchArray[0].Ptr_VectorSet->VectorNum);
//							Tmp_uint32 = TemplateMatchArray[0].Ptr_VectorSet->VectorNum;
//							for (u = 0; u < Tmp_uint32; u++) { /* ������������ */
//								printf("{ ");
//								for (v = 0; v < FEATURE_DIMENSION; v++) {  /* һ����������������ֵ���� */
//									if (v == FEATURE_DIMENSION - 1) {
//										printf("%f  ", TemplateMatchArray[0].Ptr_VectorSet->BeginPtr[u].Feature[v]);
//									} else {
//										printf("%f,  ", TemplateMatchArray[0].Ptr_VectorSet->BeginPtr[u].Feature[v]);
//									}
//								}
//								if (u == Tmp_uint32 - 1) {
//									printf("}\r\n");
//								} else {
//									printf("},\r\n");
//								}	
//					}
					//TEST
					TemplateMatchArray[i].dtw_result = DTW_MinDistance(TemplateMatchArray[i].Ptr_VectorSet, &MFCCSet_Testing);
					//printf("\t\t- ģ�� %u : %f\t", i, TemplateMatchArray[i].dtw_result);
					TemplateMatchArray[i].dtw_result = (TemplateMatchArray[i].dtw_result / (TemplateMatchArray[i].Ptr_VectorSet->VectorNum + MFCCSet_Testing.VectorNum) ) * DTW_COEFF_AMP;
					printf("\t\t- ģ�� %u : ", i+1);
					printf("%f\t%u\t%u\r\n",TemplateMatchArray[i].dtw_result, TemplateMatchArray[i].Ptr_VectorSet->VectorNum, MFCCSet_Testing.VectorNum);
				}
				
				/* ��־λ��״̬���Ʊ��� */
				WorkStage_VR = VOICE_RESUALT;
				printf("\t\t- Finish!\r\n");
				
				/* ��֤����-�������� */
				#ifdef VOICE_RECOGNITION_STEP_VERIFY
					while(Flag_VR_StepVerify);
					Flag_VR_StepVerify = 1;
				#endif
				break;
				
			  case VOICE_RESUALT :						/* �����ʾ */
				/* �������ʾ��������: A-05 */
				ASIO_Tube_Set(5, 0xC, POINT_OFF);
				ASIO_Tube_Set(4, 0x5, POINT_OFF);
				/* ����������ʾ */
				printf("\t- State : ������\r\n");
			
				/* �����ȶԽ�� */
				Tmp_float = TemplateMatchArray[0].dtw_result;
				Tmp_uint32 = 0;
				for (i = 1; i < TEMPLATE_NUM; i++) {
					if (TemplateMatchArray[i].dtw_result < Tmp_float) {
						Tmp_float = TemplateMatchArray[i].dtw_result;
						Tmp_uint32 = i;
					}
				}
				if (TemplateMatchArray[Tmp_uint32].dtw_result < DTW_THRESHOLD) {
					printf("\t\t- �ؼ��� ������ %u ", Tmp_uint32+1);
					if(load_flag==1){
						switch (Tmp_uint32) {
							case 0 : 
								printf(": Lv Yu Xuan\r\n");
								break;
							case 1 : 
								printf(": Liu Rui\r\n");
								break;
							case 2 : 
								printf(": Shao Yi\r\n");
								break;
							case 3 : 
//								printf(": Ji Chen Dian Lu\r\n");
								printf(": An Lu Ke Ji\r\n");
								break;
						}
					}
					else{
						;
						}
					/* �������ʾ��� */
					//ASIO_Tube_Set(1, 0, POINT_OFF);
					ASIO_Tube_Set(0, Tmp_uint32+1, POINT_OFF);
					/* ��־λ��״̬���Ʊ��� */
					WorkStage_VR = VOICE_IDLE;	
				}else {
					printf("\t\t- û��ƥ���ģ��!\r\n");
					/* �������ʾ��� */
					//ASIO_Tube_Set(1, 0xf, POINT_OFF);
					ASIO_Tube_Set(0, 0x0, POINT_OFF);
					/* ��־λ��״̬���Ʊ��� */
					WorkStage_VR = VOICE_IDLE;
					Flag_Idle_Display_VR = 1;
				}
				
				/* ��֤����-�������� */
				#ifdef VOICE_RECOGNITION_STEP_VERIFY
					while(Flag_VR_StepVerify);
					Flag_VR_StepVerify = 1;
				#endif
				break;
		}
	}
	return 0;
}

