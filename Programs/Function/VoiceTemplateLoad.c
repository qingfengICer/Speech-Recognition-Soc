#include "Function.h"
#include "TemplateSet.h"

int VoiceTemplateLoad_Main(void)
{
	uint32_t i, j;
	uint32_t Tmp;

	printf(" \r\n\t #模板加载\r\n");
	printf("\t- 默认模板数量：4\r\n");
	
/* 加载模板01 */
	//printf("\t- 加载模板 00\r\n");
	Tmp = sizeof(TemplateFeature_00) / sizeof(DTW_FeatureVector_Type);
	TemplateMatchArray[0].Ptr_VectorSet->VectorNum = Tmp;
	for (i = 0; i < Tmp; i++) { /* 特征向量数量 */
		for (j = 0; j < FEATURE_DIMENSION; j++) {  /* 一个特征向量中特征值个数 */
			TemplateMatchArray[0].Ptr_VectorSet->BeginPtr[i].Feature[j] = TemplateFeature_00[i].Feature[j];
		}
	}
	
	/* 加载模板02 */
	//printf("\t- 加载模板 01\r\n");
	Tmp = sizeof(TemplateFeature_01) / sizeof(DTW_FeatureVector_Type);
	TemplateMatchArray[1].Ptr_VectorSet->VectorNum = Tmp;
	for (i = 0; i < Tmp; i++) { /* 特征向量数量 */
		for (j = 0; j < FEATURE_DIMENSION; j++) {  /* 一个特征向量中特征值个数 */
			TemplateMatchArray[1].Ptr_VectorSet->BeginPtr[i].Feature[j] = TemplateFeature_01[i].Feature[j];
		}
	}
	
	/* 加载模板03 */
	//printf("\t- 加载模板 02\r\n");
	Tmp = sizeof(TemplateFeature_02) / sizeof(DTW_FeatureVector_Type);
	TemplateMatchArray[2].Ptr_VectorSet->VectorNum = Tmp;
	for (i = 0; i < Tmp; i++) { /* 特征向量数量 */
		for (j = 0; j < FEATURE_DIMENSION; j++) {  /* 一个特征向量中特征值个数 */
			TemplateMatchArray[2].Ptr_VectorSet->BeginPtr[i].Feature[j] = TemplateFeature_02[i].Feature[j];
		}
	}
	
	/* 加载模板04 */
	//printf("\t- 加载模板 03\r\n");
	Tmp = sizeof(TemplateFeature_03) / sizeof(DTW_FeatureVector_Type);
	TemplateMatchArray[3].Ptr_VectorSet->VectorNum = Tmp;
	for (i = 0; i < Tmp; i++) { /* 特征向量数量 */
		for (j = 0; j < FEATURE_DIMENSION; j++) {  /* 一个特征向量中特征值个数 */
			TemplateMatchArray[3].Ptr_VectorSet->BeginPtr[i].Feature[j] = TemplateFeature_03[i].Feature[j];
		}
	}
	
	printf("\t- 默认模板加载完毕！\r\n");
	
	return 1;
}

