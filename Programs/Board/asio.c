#include "asio.h"
#include <stdio.h>

void ASIO_Tube_Set(unsigned char tubex, unsigned char data, ASIO_TubePoint_TypeDef POINT)
{
    ASIO->TUBEMASK |= (1 << tubex);
    switch (tubex)
    {
        case 0:
            ASIO->TUBE0 = (unsigned char)((data & 0x0F) | (POINT << 4));
            break;
        case 1:
            ASIO->TUBE1 = (unsigned char)((data & 0x0F) | (POINT << 4));
            break;
        case 2:
            ASIO->TUBE2 = (unsigned char)((data & 0x0F) | (POINT << 4));
            break;
        case 3:
            ASIO->TUBE3 = (unsigned char)((data & 0x0F) | (POINT << 4));
            break;
        case 4:
            ASIO->TUBE4 = (unsigned char)((data & 0x0F) | (POINT << 4));
            break;
        case 5:
            ASIO->TUBE5 = (unsigned char)((data & 0x0F) | (POINT << 4));
            break;
    }
    return;
}

/* 按键中断函数示例 */
void KEY_IRQHandler(void)
{
    static unsigned char i = 0;
    
    i = (i < 0xF) ? (i + 1) : 0;
    ASIO_Tube_Set(1, i, POINT_OFF);
    printf("Test\r\n");
    printf("i = %d\r\n", i);

    ASIO_KeyInt_Clear();
    return;
}

