
#ifndef _VOICEPREPROCESSING_H_
#define _VOICEPREPROCESSING_H_

#include <stdint.h>
#include "sdram.h"
#include "fft.h"

#define VAD_FRAME_LEN           256UL
#define VAD_THRESHOLD           (VAD_FRAME_LEN * 180 * 180)
#define VAD_SEGMENT_NUM_MAX			(10 * 2)

#define EMPHASIS_FACTOR         0.98

typedef struct {
	SDRAM_Type_int32 VoiceSegment[VAD_SEGMENT_NUM_MAX];
	uint32_t SegmentNum;
}	VoiceSegmentRecorder_Type;

int32_t VoiceActivityDetection_Multi(int32_t *AudioBegin, uint32_t AudioLen, VoiceSegmentRecorder_Type *VoiceSegmentRecorder);
void VoiceSegmentSplicing(VoiceSegmentRecorder_Type *VoiceSegmentRecorder, SDRAM_Type_int32 *VoiceSplicedData);
void VoicePreEmphasis(int32_t *VoiceBegin, uint32_t VoiceLen);
float CalculateFactor_Hann(uint32_t WinLen, uint32_t Index);
void FrameCopy_HannWin(int32_t *SrcPtr, Complex_Type *DstPtr, uint32_t FrameLen);

#endif 
