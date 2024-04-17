
#include "dtw.h"
#include "stdio.h"


static float EuclideanDistance(uint32_t VectorLen, float *Vector_1, float *Vector_2)
{
	uint32_t i;
	float Tmp;
	float Sum = 0;
	for (i = 0; i < VectorLen; i++) {
		Tmp = (Vector_1[i] - Vector_2[i]);
		Sum += Tmp * Tmp;
	}
	return (float)sqrtf(Sum);
}

static float GetMinSum(float Current, float Left, float Diagonal, float Below)
{
	float Sum_Left = Current + Left;
	float Sum_Diagonal = Current + Diagonal;
	float Sum_Below = Current + Below;
	float Sum_Min;
	if (Sum_Left <= Sum_Diagonal && Sum_Left <= Sum_Below) {
		Sum_Min = Sum_Left;
	} else if (Sum_Diagonal <= Sum_Left && Sum_Diagonal <= Sum_Below) {
		Sum_Min = Sum_Diagonal;
	} else {
		Sum_Min = Sum_Below;
	}
	return Sum_Min;
}

float DTW_MinDistance(DTW_VectorSet_Type *Template, DTW_VectorSet_Type *Testing)
{
	uint32_t x, y;
	float TmpSumArry[2][Template->VectorNum];
	float CurrentEuclidean;
	/* 计算第0行各点的最小累加和 */
	TmpSumArry[0][0] = EuclideanDistance(FEATURE_DIMENSION, Template->BeginPtr[0].Feature, Testing->BeginPtr[0].Feature);
	for (x = 1; x < Template->VectorNum; x++) {
		CurrentEuclidean = EuclideanDistance(FEATURE_DIMENSION, Template->BeginPtr[x].Feature, Testing->BeginPtr[0].Feature);
		TmpSumArry[0][x] = CurrentEuclidean + TmpSumArry[0][x - 1];
	}
	/* 计算 1 ~ Testing->VectorNum 行各点的最小累加和 */
	for (y = 1; y < Testing->VectorNum; y++) {
		/* 计算当前行各点的最小累加和 */
		CurrentEuclidean = EuclideanDistance(FEATURE_DIMENSION, Template->BeginPtr[0].Feature, Testing->BeginPtr[y].Feature);
		TmpSumArry[1][0] = CurrentEuclidean + TmpSumArry[0][0];
		for (x = 1; x < Template->VectorNum; x++) {
			CurrentEuclidean = EuclideanDistance(FEATURE_DIMENSION, Template->BeginPtr[x].Feature, Testing->BeginPtr[y].Feature);
			TmpSumArry[1][x] = GetMinSum(CurrentEuclidean, TmpSumArry[1][x - 1], TmpSumArry[0][x - 1], TmpSumArry[0][x]);
		}
		/* 准备计算下一行 */
		for (x = 0; x < Template->VectorNum; x++) {
			TmpSumArry[0][x] = TmpSumArry[1][x];
		}
	}
	return TmpSumArry[1][Template->VectorNum - 1];
}
