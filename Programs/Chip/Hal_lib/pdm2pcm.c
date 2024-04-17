

#include "pdm2pcm.h"
#include <stdio.h>


void pdm2pcm_enable(void)
{
	PDM2PCM->INTR = 0;
	PDM2PCM->CTRL = (0x00000000u) |              	/* FIFO clear */
									PDM2PCM_CTRL_EN_MASK |              /* Module enable */
									PDM2PCM_CTRL_FINT_EN_MASK |              /* Full Interrupt enable */
									~PDM2PCM_CTRL_HFINT_EN_MASK;              /* Half full Interrupt enable */
}

void pdm2pcm_disable(void)
{
	PDM2PCM->CTRL = 0;
	PDM2PCM->INTR = 0x03;
}


uint32_t pdm2pcm_check_fifo_empty(void)
{
	return (PDM2PCM->STATE & PDM2PCM_STAT_EMPTY_MASK);
}

uint32_t pdm2pcm_check_fifo_full(void)
{
	return (PDM2PCM->STATE & PDM2PCM_STAT_FULL_MASK);
}

uint32_t pdm2pcm_read_fifo(void)
{
	return (PDM2PCM->DATA);
}

void pdm2pcm_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* enable RX interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = PDM2PCM_Full_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* enable RX interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = PDM2PCM_Half_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void FIFO_Half_Handler(void)
{
	printf("error: fifo half full!!!\r\n");
	pdm2pcm_disable();
}

void FIFO_Full_Handler(void)
{
	printf("error: fifo full!!!\r\n");
	pdm2pcm_disable();
}
