#include "Function.h"
#include "TemplateSet.h"

int VoiceTemplateLoad_Main(void)
{
	uint32_t i, j;
	uint32_t Tmp;

	printf(" \r\n\t #ģ�����\r\n");
	printf("\t- Ĭ��ģ��������4\r\n");
	
/* ����ģ��01 */
	//printf("\t- ����ģ�� 00\r\n");
	Tmp = sizeof(TemplateFeature_00) / sizeof(DTW_FeatureVector_Type);
	TemplateMatchArray[0].Ptr_VectorSet->VectorNum = Tmp;
	for (i = 0; i < Tmp; i++) { /* ������������ */
		for (j = 0; j < FEATURE_DIMENSION; j++) {  /* һ����������������ֵ���� */
			TemplateMatchArray[0].Ptr_VectorSet->BeginPtr[i].Feature[j] = TemplateFeature_00[i].Feature[j];
		}
	}
	
	/* ����ģ��02 */
	//printf("\t- ����ģ�� 01\r\n");
	Tmp = sizeof(TemplateFeature_01) / sizeof(DTW_FeatureVector_Type);
	TemplateMatchArray[1].Ptr_VectorSet->VectorNum = Tmp;
	for (i = 0; i < Tmp; i++) { /* ������������ */
		for (j = 0; j < FEATURE_DIMENSION; j++) {  /* һ����������������ֵ���� */
			TemplateMatchArray[1].Ptr_VectorSet->BeginPtr[i].Feature[j] = TemplateFeature_01[i].Feature[j];
		}
	}
	
	/* ����ģ��03 */
	//printf("\t- ����ģ�� 02\r\n");
	Tmp = sizeof(TemplateFeature_02) / sizeof(DTW_FeatureVector_Type);
	TemplateMatchArray[2].Ptr_VectorSet->VectorNum = Tmp;
	for (i = 0; i < Tmp; i++) { /* ������������ */
		for (j = 0; j < FEATURE_DIMENSION; j++) {  /* һ����������������ֵ���� */
			TemplateMatchArray[2].Ptr_VectorSet->BeginPtr[i].Feature[j] = TemplateFeature_02[i].Feature[j];
		}
	}
	
	/* ����ģ��04 */
	//printf("\t- ����ģ�� 03\r\n");
	Tmp = sizeof(TemplateFeature_03) / sizeof(DTW_FeatureVector_Type);
	TemplateMatchArray[3].Ptr_VectorSet->VectorNum = Tmp;
	for (i = 0; i < Tmp; i++) { /* ������������ */
		for (j = 0; j < FEATURE_DIMENSION; j++) {  /* һ����������������ֵ���� */
			TemplateMatchArray[3].Ptr_VectorSet->BeginPtr[i].Feature[j] = TemplateFeature_03[i].Feature[j];
		}
	}
	
	printf("\t- Ĭ��ģ�������ϣ�\r\n");
	
	return 1;
}

