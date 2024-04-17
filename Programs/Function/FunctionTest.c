
#include "Function.h"

uint8_t TestMode = TEST_MODE_IDLE;

static void Test_UART(void);
static void Test_SDRAM (void) ;


int Test_Main(void)
{
	printf("Function: Test\r\n");
	while (FunctionMode == FUNCTION_MODE_TEST) {
		switch (TestMode) {
			case TEST_MODE_IDLE :
				
				break;
			case TEST_MODE_UART :
				printf("test uart !\r\n");
				Test_UART();
			break;
			case TEST_MODE_SDRAM :
				Test_SDRAM ();
			break;
		}
	}
	return 0;
}



static void Test_UART(void)
{
	
}

static void Test_SDRAM (void) 
{
	int i;
	int *addr;
	uint32_t *ptr_sdram_write = (uint32_t *)0x60000020;
	uint32_t *tmp_addr = ptr_sdram_write;
	
	printf("initialization finish!\r\n");
	
	addr = (int *)0x0000a000;
	*addr = 0xabcdabcd;
	
	for (i = 0; i < 128; i++) {
		*tmp_addr++ = 0x00 + i;
	}
	printf("write finish !\r\n");
	Delay_ms(2000);
	tmp_addr = ptr_sdram_write;
	for (i = 0; i < 256; i++) {
		printf("addr : %x,  value: %x\r\n", (uint32_t)(tmp_addr-1), *tmp_addr);
		tmp_addr++;
	}
	
	addr = (int *)0x0000a004;
	*addr = 0x5a5a5a5a;
	
	while(TestMode == TEST_MODE_SDRAM)
	{
		printf("1s\r\n");
		Delay_ms(1000);
//		tmp_addr = ptr_sdram_write + 256;
//		for (i = 0; i < 16; i++) {
//			*tmp_addr++ = 0x00 + i;
//		}
//		Delay_ms(2000);
//		tmp_addr = ptr_sdram_write + 256;
//		for (i = 0; i < 16; i++) {
//			sdram_read[i] = *tmp_addr++ ;
//			printf("addr : %x,  value: %x\r\n", (uint32_t)(tmp_addr-1), sdram_read[i]);
//		}
	}
}
