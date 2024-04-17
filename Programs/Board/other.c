
/* 寄存器定义结构体 */
typedef struct
{
    __IO    uint8_t     KEY;
            uint8_t     RESERVED0;
            uint16_t    RESERVED1;

    __IO    uint8_t     KEYINT;
            uint8_t     RESERVED2;
            uint16_t    RESERVED3;

    __I     uint16_t    SWITCH;
            uint16_t    RESERVED4;

    __IO    uint16_t    LED;
            uint16_t    RESERVED5;

    __IO    uint8_t     TUBEMASK;
            uint8_t     RESERVED6;
            uint16_t    RESERVED7;

    __IO    uint8_t     TUBE0;
            uint8_t     RESERVED8;
            uint16_t    RESERVED9;

    __IO    uint8_t     TUBE1;
            uint8_t     RESERVED10;
            uint16_t    RESERVED11;

    __IO    uint8_t     TUBE2;
            uint8_t     RESERVED12;
            uint16_t    RESERVED13;

    __IO    uint8_t     TUBE3;
            uint8_t     RESERVED14;
            uint16_t    RESERVED15;

    __IO    uint8_t     TUBE4;
            uint8_t     RESERVED16;
            uint16_t    RESERVED17;

    __IO    uint8_t     TUBE5;
            uint8_t     RESERVED18;
            uint16_t    RESERVED19;

            uint32_t    RESERVED20[5];
} ASIO_TypeDef;

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
