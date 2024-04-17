#ifndef _DTW_H_
#define _DTW_H_

#include <stdint.h>
#include <math.h>

#define FEATURE_DIMENSION			14

typedef struct {
	float Feature[FEATURE_DIMENSION];
} DTW_FeatureVector_Type;

typedef struct {
	DTW_FeatureVector_Type *BeginPtr;
	uint32_t VectorNum;
} DTW_VectorSet_Type;

float DTW_MinDistance(DTW_VectorSet_Type *Template, DTW_VectorSet_Type *Testing);

#endif 
