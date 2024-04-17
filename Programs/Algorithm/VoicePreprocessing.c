
#include "VoicePreprocessing.h"
#include <stdio.h>
#include "fft.h"

/*
 * @brf     : detect valid voice from input audio data.
 * @para    : *AudioBegin - pointer of the first address of input audio data.
 *            AudioLen - length of input audio data.
 *            *VoiceBegin - pointer of the first address of valid voice in the audio data.
 *            VoiceLen - length of valid voice.
 * @return  : 0 - there are valid voice datas in input audio data.
 *            others - no valid voice datas in input audio data.
 * @note    : *AudioBegin & AudioLen are input parameters, while *VoiceBegin & VoiceLen are output parameters.
 */
int32_t VoiceActivityDetection_Multi(int32_t *AudioBegin, uint32_t AudioLen, VoiceSegmentRecorder_Type *VoiceSegmentRecorder)
{
    uint32_t i ;
    uint32_t  Sum = 0;
		uint32_t Flag_Segment = 0;
	
		VoiceSegmentRecorder->VoiceSegment[0].BeginPtr = NULL;
		VoiceSegmentRecorder->SegmentNum = 0;
    
    for (i = 0; i < VAD_FRAME_LEN; i++) {
        Sum += (uint32_t)(AudioBegin[i] * AudioBegin[i]);
    }
    for (i = 1; i < AudioLen - VAD_FRAME_LEN + 1; i++) {
        Sum -= (uint32_t)(AudioBegin[i - 1] * AudioBegin[i - 1]);
        Sum += (uint32_t)(AudioBegin[i + VAD_FRAME_LEN - 1] * AudioBegin[i + VAD_FRAME_LEN - 1]);
        if (Sum > VAD_THRESHOLD) {
						if (Flag_Segment == 0) {
								VoiceSegmentRecorder->VoiceSegment[VoiceSegmentRecorder->SegmentNum].BeginPtr = &AudioBegin[i];
						}
						Flag_Segment = 1;
				} else {
						if (Flag_Segment == 1) {
								VoiceSegmentRecorder->VoiceSegment[VoiceSegmentRecorder->SegmentNum].DataNum = &AudioBegin[i] - VoiceSegmentRecorder->VoiceSegment[VoiceSegmentRecorder->SegmentNum].BeginPtr;
								VoiceSegmentRecorder->SegmentNum++;
								if (VoiceSegmentRecorder->SegmentNum == VAD_SEGMENT_NUM_MAX && i < AudioLen - VAD_FRAME_LEN + 1) {
									printf("\t\t- segment over!!!!\r\n");
									return -2;
								} 
						}
						Flag_Segment = 0;
				}
    }
   
    if (VoiceSegmentRecorder->VoiceSegment[0].BeginPtr == NULL || VoiceSegmentRecorder->SegmentNum == 0) {
        return -1;
    } else {
			printf("\t\t- voice segment: %u\r\n", VoiceSegmentRecorder->SegmentNum);
        return 0;
    }
}

void VoiceSegmentSplicing(VoiceSegmentRecorder_Type *VoiceSegmentRecorder, SDRAM_Type_int32 *VoiceSplicedData)
{
		uint32_t i, j;
		VoiceSplicedData->DataNum = 0;
		for (i = 0; i < VoiceSegmentRecorder->SegmentNum; i++) {
				for (j = 0; j < VoiceSegmentRecorder->VoiceSegment[i].DataNum; j++) {
						VoiceSplicedData->BeginPtr[VoiceSplicedData->DataNum] = VoiceSegmentRecorder->VoiceSegment[i].BeginPtr[j];
						VoiceSplicedData->DataNum++;
				}
		} 
}

/*
 * @brf     : pre-emphasis input voice data to enhance amplitude of high frequency.
 * @para    : *VoiceBegin - pointer of the first address of valid voice in the audio data.
 *            VoiceLen - length of valid voice.
 *            EmphasisFactor - factor of high pass filter
 * @return  : none
 * @note    : *VoiceBegin & VoiceLen are both input and output parameters.
 */
void VoicePreEmphasis(int32_t *VoiceBegin, uint32_t VoiceLen)
{
    uint32_t i;
		int32_t  LastData = VoiceBegin[0];
		int32_t  CrntData;
    for (i = 1; i < VoiceLen; i++) {
				CrntData = VoiceBegin[i];
        VoiceBegin[i] = CrntData + (int32_t)(LastData * EMPHASIS_FACTOR);
				LastData = CrntData;
    }
}

/*
 * @brf     : calculate the Hann coefficient to multiply the index-th element of the window.
 * @para    : WinLen - data numbers of window.
 *            Index - index in window.
 * @return  : the coefficient to multiply the index-th element.
 * @note    : F(n) = 0.5*(1 - cos(2*π*n/N))    0 ≤ n ≤ N
 */
float CalculateFactor_Hann(uint32_t WinLen, uint32_t Index)
{ 
    return (0.5 * (1 - GetCosineValue(Index * 2 * PI / WinLen)));
}


void FrameCopy_HannWin(int32_t *SrcPtr, Complex_Type *DstPtr, uint32_t FrameLen)
{
    uint32_t i;
    for (i = 0; i < FrameLen; i++) {
        DstPtr[i].real = SrcPtr[i] * CalculateFactor_Hann(FrameLen, i);
        DstPtr[i].image = 0;
    }
}
