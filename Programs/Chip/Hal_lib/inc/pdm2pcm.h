
#ifndef _PDM2PCM_H_
#define _PDM2PCM_H_

#include <stdint.h>
#include "soc.h"

void pdm2pcm_init(void);
void pdm2pcm_enable(void);
void pdm2pcm_disable(void);

uint32_t pdm2pcm_check_fifo_empty(void);
uint32_t pdm2pcm_check_fifo_full(void);
uint32_t pdm2pcm_read_fifo(void);

#endif
