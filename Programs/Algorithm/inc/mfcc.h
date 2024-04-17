
#ifndef _MFCC_H_
#define _MFCC_H_

#include <stdint.h>
#include <math.h>
#include "Function.h"
#include "fft.h"

#ifndef FFT_N
	#define FFT_N		256
#endif
#ifndef FFT_N
	#define SAMPLE_FREQ		8000
#endif
#define MEL_FILTER_NUM                  26
#define VALID_FREQ_MIN                  0
#define VALID_FREQ_MAX                  4000

void MFCC_SetFilterTable(void);
void MFFC_FeatureAbstract(Complex_Type *Ptr_fft, uint32_t Len_fft, uint32_t FilterNum, float *Result_Ptr, uint32_t Result_Len);

#endif 
