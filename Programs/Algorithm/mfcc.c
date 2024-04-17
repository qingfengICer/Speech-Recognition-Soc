
#include "mfcc.h"


static uint32_t FilterPointTable[MEL_FILTER_NUM + 2];

#define MEL_SCALE_MIN                   (2595 * log10(1 + (double)VALID_FREQ_MIN / 700.0))
#define MEL_SCALE_MAX                   (2595 * log10(1 + (double)VALID_FREQ_MAX / 700.0))
#define MEL_SCALE_DELTA                 ((MEL_SCALE_MAX - MEL_SCALE_MIN) / (MEL_FILTER_NUM + 1))
#define MEL_TO_FREQ(Mel)                (700 * (pow(10, (Mel) / 2595) - 1))
#define FREQ_TO_POINT(F)                ((uint32_t)((F) * ((double)FFT_N) / ((double)SAMPLE_FREQ)))
void MFCC_SetFilterTable(void)
{
	uint32_t i ;
	float Parameter_Mel[MEL_FILTER_NUM+2];
	float Parameter_Freq[MEL_FILTER_NUM + 2];
	
//	printf("print mel filter table!\r\n");
	for (i = 0; i < MEL_FILTER_NUM + 2; i++)
	{
			Parameter_Mel[i] = MEL_SCALE_MIN + MEL_SCALE_DELTA * i;
			Parameter_Freq[i] = MEL_TO_FREQ(Parameter_Mel[i]);
			FilterPointTable[i] = FREQ_TO_POINT(Parameter_Freq[i]);
//			printf("%d\t%f\t%f\t%u\r\n", i, Parameter_Freq[i], Parameter_Mel[i], FilterPointTable[i]);
	}
}

static float MelFilter(Complex_Type *Ptr, uint32_t StartPoint, uint32_t PeakPoint, uint32_t StopPoint)
{
	uint32_t i;
	float Sum = 0;
	for (i = StartPoint + 1; i < PeakPoint; i++) {
			Sum += COMPLEX_ABS_SQR(Ptr[i].real, Ptr[i].image) * ((float)(i - StartPoint) / (PeakPoint - StartPoint));
	}
	for (i = PeakPoint; i < StopPoint; i++) {
			Sum += COMPLEX_ABS_SQR(Ptr[i].real, Ptr[i].image) * ((float)(StopPoint - i) / (StopPoint - PeakPoint));
	}
	return (Sum <= 0 ? 0 : log(Sum));
}

static float DiscreteCosineTransform(float *Ptr, uint32_t Len, uint32_t index)
{
	uint32_t i;
	float Sum = 0;
	for (i = 0; i < Len; i++) {
			Sum += Ptr[i] * GetCosineValue(PI * index * (i - 0.5) / Len);
	}
	return Sum;
}

void MFFC_FeatureAbstract(Complex_Type *Ptr_fft, uint32_t Len_fft, uint32_t FilterNum, float *Result_Ptr, uint32_t Result_Len)
{
	uint32_t i;
	float Sum = 0;
	float FilterOut[FilterNum];
	
	/* Mel filter & do log */
	for (i = 0; i < FilterNum; i++) {
			FilterOut[i] = MelFilter(Ptr_fft, FilterPointTable[i], FilterPointTable[i+1], FilterPointTable[i+2]);
	}
	/* DCT */
	for (i = 0; i < Result_Len - 1; i++) {
			Result_Ptr[i] = DiscreteCosineTransform(FilterOut, FilterNum, i+1);
	}
	/* log10 of frame energe */
	for (i = 0; i < Len_fft; i++)
	{
			Sum += COMPLEX_ABS_SQR(Ptr_fft[i].real, Ptr_fft[i].image);
	}
	Result_Ptr[Result_Len - 1] = (Sum <= 0 ? 0 : log10(Sum));
}

