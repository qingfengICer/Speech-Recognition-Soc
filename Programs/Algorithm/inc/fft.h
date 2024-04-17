
#ifndef _FFT_H_
#define _FFT_H_

#include <stdint.h>
#include <math.h>

#define PI 3.14159265358979323846264338327950288419716939937510L /* π */


#define DIVIDE_NUM_2PI          256U
#define DIVIDE_ACCURACY_2PI     (2 * PI / DIVIDE_NUM_2PI)
#define SINE_TABLE_SIZE         (DIVIDE_NUM_2PI / 4 + 1)

#define COMPLEX_ABS_SQR(real, image)                    (((real) * (real)) + ((image) * (image)))
#define FFT_RESUALT_FREQ(Fs, fft_n, index)							((float)(index) * (Fs) / (fft_n))


typedef struct {
    float real;
    float image;
} Complex_Type;

void CreateSineTable(void);
float GetSineValue(float Radian);
float GetCosineValue(float Radian);
void Transform_FFT(Complex_Type fft_data[], uint32_t fft_n);
float FFT_GetResult_Amp(uint32_t fft_n, uint32_t index, Complex_Type fft_data);

#endif


