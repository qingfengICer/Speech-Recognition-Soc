
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
	
	printf("\r\n\t #ģ��ɼ�\r\n");
	printf("\t - ����ʱ��: %u s\r\n", SAMPLE_TIME);
	
	while(FunctionMode == FUNCTION_MODE_TEMPLATE_ACQUISITION) {
		switch (WorkStage_VTA) {
			case VOICE_IDLE :
				if (Flag_Idle_Display_VTA) {
					/* �������ʾ��������: C-00 */
				
					/* ���ڴ�ӡ�������� */
					printf("\t- State : ����\r\n");
					Flag_Idle_Display_VTA = 0;
				}
				
				if (Flag_VTA_Start && Template_Current != 0) {
					VoiceSplicedData.DataNum = 0;
					MFCCSet_Testing.VectorNum = 0;
					WorkStage_VTA = VOICE_COLLECTION;
					Flag_VTA_Start = 0;
				}
				break;
				
			case VOICE_COLLECTION :				/* ��Ƶ�ɼ� */
				/* �������ʾ��������: C-01 */
			
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
				
				/* ������ת��Ϊ��0Ϊ���� */
				AudioAverage /= (int32_t)AudioData.DataNum;
				for (i = 0; i < AudioData.DataNum; i++) {
					AudioData.BeginPtr[i] -= AudioAverage;
				}

				WorkStage_VTA = VOICE_PREPROCESSING;
				printf("\t\t- Finish!\r\n");
				break;
				
			case VOICE_PREPROCESSING :			/* Ԥ���� */
				/* �������ʾ��������: C-02 */
			
				/* ����������ʾ */
				printf("\t- State : ��Ƶ���ݴ���\r\n");
				/* �����μ�� & Ԥ���� */
				if (VoiceActivityDetection_Multi(AudioData.BeginPtr, AudioData.DataNum, &VoiceSegmentRecorder) != -1) {
					
					/* ������ƴ�� */
					VoiceSegmentSplicing(&VoiceSegmentRecorder, &VoiceSplicedData);
					
					/* Ԥ���� */
					VoicePreEmphasis(VoiceSplicedData.BeginPtr, VoiceSplicedData.DataNum);
					
					/* ��־λ��״̬���Ʊ��� */
					WorkStage_VTA = VOICE_MFCC;
					printf("\t\t- Finish!\r\n");
				} else {
					printf("\t\tδ��⵽��Ч����!\r\n");
					WorkStage_VTA = VOICE_IDLE;
					Flag_Idle_Display_VTA = 1;
				}
				break;
				
			case VOICE_MFCC :						/* MFCC����������ȡ */
				/* �������ʾ��������: C-03 */
			
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
				
				/* ��־λ��״̬���Ʊ��� */
				WorkStage_VTA = VOICE_MFCC_STORE;
				printf("\t\t- Finish!\r\n");
				break;
				
				case VOICE_MFCC_STORE :		/* ģ��洢 */
					/* �������ʾ��������: C-04 */
			
					/* ����������ʾ */
					printf("\t- State : ��ƵFeature�洢\r\n");
				
					/* ģ��洢 */
					switch (Template_Current) {
						case 1 :
							Tmp_uint32 = MFCCSet_Testing.VectorNum;
							TemplateMatchArray[0].Ptr_VectorSet->VectorNum  = Tmp_uint32;
							for (i = 0; i < Tmp_uint32; i++) { /* ������������ */
								for (j = 0; j < FEATURE_DIMENSION; j++) {  /* һ����������������ֵ���� */
									TemplateMatchArray[0].Ptr_VectorSet->BeginPtr[i].Feature[j] = MFCCSet_Testing.BeginPtr[i].Feature[j];
								}
							}
							break;
							
						case 2 :
							Tmp_uint32 = MFCCSet_Testing.VectorNum;
							TemplateMatchArray[1].Ptr_VectorSet->VectorNum  = Tmp_uint32;
							for (i = 0; i < Tmp_uint32; i++) { /* ������������ */
								for (j = 0; j < FEATURE_DIMENSION; j++) {  /* һ����������������ֵ���� */
									TemplateMatchArray[1].Ptr_VectorSet->BeginPtr[i].Feature[j] = MFCCSet_Testing.BeginPtr[i].Feature[j];
								}
							}
							break;
							
						case 3 :
							Tmp_uint32 = MFCCSet_Testing.VectorNum;
							TemplateMatchArray[2].Ptr_VectorSet->VectorNum  = Tmp_uint32;
							for (i = 0; i < Tmp_uint32; i++) { /* ������������ */
								for (j = 0; j < FEATURE_DIMENSION; j++) {  /* һ����������������ֵ���� */
									TemplateMatchArray[2].Ptr_VectorSet->BeginPtr[i].Feature[j] = MFCCSet_Testing.BeginPtr[i].Feature[j];
								}
							}
							break;
							
						case 4 :
							Tmp_uint32 = MFCCSet_Testing.VectorNum;
							TemplateMatchArray[3].Ptr_VectorSet->VectorNum  = Tmp_uint32;
							for (i = 0; i < Tmp_uint32; i++) { /* ������������ */
								for (j = 0; j < FEATURE_DIMENSION; j++) {  /* һ����������������ֵ���� */
									TemplateMatchArray[3].Ptr_VectorSet->BeginPtr[i].Feature[j] = MFCCSet_Testing.BeginPtr[i].Feature[j];
								}
							}
							break;
							
						case 0xff :
							printf("\t\t- ģ����չʾ : %u\r\n", MFCCSet_Testing.VectorNum);
							Tmp_uint32 = MFCCSet_Testing.VectorNum;
							for (i = 0; i < Tmp_uint32; i++) { /* ������������ */
								printf("{ ");
								for (j = 0; j < FEATURE_DIMENSION; j++) {  /* һ����������������ֵ���� */
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
					/* ��־λ��״̬���Ʊ��� */
				WorkStage_VTA = VOICE_IDLE;
				Flag_Idle_Display_VTA = 1;
				printf("\t\t- Finish!\r\n");
				break;
		} 
	}
	
	return 0;
}

