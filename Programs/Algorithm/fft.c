
#include "fft.h"
#include "stdio.h"

static float SineTable[SINE_TABLE_SIZE] = {0};

void CreateSineTable(void)
{
    uint32_t i;
    for ( i = 0; i < SINE_TABLE_SIZE; i++) {
        SineTable[i] = (float)sin((double)(DIVIDE_ACCURACY_2PI * i));
    }
}

float GetSineValue(float Radian)
{
    float tmp;
    uint32_t index;
		/* Radian 为负时将其转换为等价的正值 */
		if (Radian < 0) Radian = 0 - Radian + PI;
		
    tmp = Radian / DIVIDE_ACCURACY_2PI;
    if (tmp - (uint32_t)tmp > DIVIDE_ACCURACY_2PI / 2) {
        index = (uint32_t)tmp + 1;
    } else {
        index = (uint32_t)tmp;
    }
    while(index >= DIVIDE_NUM_2PI) index -= DIVIDE_NUM_2PI;
    if (index <= (DIVIDE_NUM_2PI >> 2)) {
        tmp = SineTable[index];
    } else if (index <= (DIVIDE_NUM_2PI >> 1)) {
        tmp = SineTable[(DIVIDE_NUM_2PI >> 1) - index];
    } else if (index <= 3*(DIVIDE_NUM_2PI >> 2)) {
        tmp = 0 - SineTable[index - (DIVIDE_NUM_2PI >> 1)];
    } else {
        tmp = 0 - SineTable[DIVIDE_NUM_2PI - index];
    }
    return tmp;
}

float GetCosineValue(float Radian)
{
    float tmp;
    uint32_t index;
	
		/* Radian 为负时将其转换为等价的正值 */
		if (Radian < 0) Radian = 0 - Radian ;
	
    tmp = Radian / DIVIDE_ACCURACY_2PI;
    if (tmp - (uint32_t)tmp > DIVIDE_ACCURACY_2PI / 2) {
        index = (uint32_t)tmp + 1;
    } else {
        index = (uint32_t)tmp;
    }
    while(index >= DIVIDE_NUM_2PI) index -= DIVIDE_NUM_2PI;
    if (index <= (DIVIDE_NUM_2PI >> 2)) {
        tmp = SineTable[(DIVIDE_NUM_2PI >> 2) - index];
    } else if (index <= (DIVIDE_NUM_2PI >> 1)) {
        tmp = 0 - SineTable[index - (DIVIDE_NUM_2PI >> 2)];
    } else if (index <= 3*(DIVIDE_NUM_2PI >> 2)) {
        tmp = 0 - SineTable[3*(DIVIDE_NUM_2PI >> 2) - index];
    } else {
        tmp = SineTable[index - 3*(DIVIDE_NUM_2PI >> 2)];
    }
    return tmp;
}

void Transform_FFT(Complex_Type fft_data[], uint32_t fft_n)
{
    uint32_t stage; /* 需要计算的级数 */
    uint32_t sub_dft; /* 某一级中要计算DFT的组数 */
    uint32_t buterfly; /* 某一组中蝶形公式的数量 */
    Complex_Type twiddleFactor; /* 存储旋转因子的值 */
    Complex_Type temp_complex;  /* 存储与旋转因子相关乘积项的值 */
    uint32_t temp_int01, temp_int02;
    uint32_t i, j, k;

    /* 按倒位序重排数组元素的位置。到位序与自然顺序相反，高位累加向低位进位。
    进行重排的数组元素个数必须为2^n个 */
    j = 0; /* 从0开始依次计算到位序的二进制编码 */
    for (i = 1; i < fft_n - 1; i++) {

        /* 计算i位置对应的到位序编码，左边加1向右进位 */
        k = fft_n >> 1; /* 从最高位开始比对 */
        while (j >= k) { /* j>=k表明对应位为1，否则对应位为0 */
            j = j - k; /* 将对应位清零 */
            k >>= 1; /* 准备比较下一位 */
        }
        j = j + k; /* 将对应位置1 */

        /* 对换复合条件的元素的位置 */
        if (j > i) { /* j=i无需对换，j<i时已经对换过了 */
            temp_complex = fft_data[j];
            fft_data[j] = fft_data[i];
            fft_data[i] = temp_complex;
        }
    }

    /* 计算蝶形算法的级数 */
    for (stage = 1, i = fft_n; (i >>= 1) != 1; stage++) ;

    /* 按蝶形算法一级一级作傅里叶变换 */
    for (i = 1; i <= stage; i++) {
        sub_dft = fft_n >> i; /* 计算第i级中要计算DFT的组数 */
        buterfly = 1 << (i - 1); /* 计算第i级各组中蝶形公式的数量 */

        /* 每一级有sub_dft组数据要作DFT变换 */
        for (j = 0; j < sub_dft; j++) {

            /* 每一组中有buterfly个蝶形公式要计算 */
            for (k = 0; k < buterfly; k++) {

                /* 每一个蝶形公式有一个旋转因子 */
                temp_int01 = k * sub_dft; /* 计算旋转因子的幂次, 小于FFT_N/2 */
                temp_int02 = DIVIDE_NUM_2PI / fft_n; /* 查找正弦函数表的放大系数 */
                if (temp_int01 > (fft_n >> 2)) {
                    twiddleFactor.real = 0 - SineTable[(temp_int01 - (fft_n >> 2)) * temp_int02];
                    twiddleFactor.image = SineTable[((fft_n >> 1) - temp_int01) * temp_int02];
                } else {
                    twiddleFactor.real = SineTable[((fft_n >> 2) - temp_int01) * temp_int02];
                    twiddleFactor.image = SineTable[temp_int01 * temp_int02];
                }

                /* 一个蝶形公式的计算 */
                temp_int01 = k + j * (buterfly << 1);
                temp_int02 = temp_int01 + buterfly; /* 计算与旋转因子相乘数据的下标 */
                temp_complex.real = fft_data[temp_int02].real * twiddleFactor.real - fft_data[temp_int02].image * twiddleFactor.image;
                temp_complex.image = fft_data[temp_int02].real * twiddleFactor.image + fft_data[temp_int02].image * twiddleFactor.real;
                fft_data[temp_int02].real = fft_data[temp_int01].real - temp_complex.real;
                fft_data[temp_int02].image = fft_data[temp_int01].image - temp_complex.image;
                fft_data[temp_int01].real = fft_data[temp_int01].real + temp_complex.real;
                fft_data[temp_int01].image = fft_data[temp_int01].image + temp_complex.image;
            }
        }
    }
}

float FFT_GetResult_Amp(uint32_t fft_n, uint32_t index, Complex_Type fft_data)
{
		float tmp = 2 * sqrtf(fft_data.real * fft_data.real + fft_data.image * fft_data.image) / fft_n;
		if (index == 0 || index == fft_n/2) {
				tmp /= 2;
		} 
		return tmp;
}
