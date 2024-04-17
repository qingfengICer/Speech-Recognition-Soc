#ifndef __ASIO

    #define __ASIO

    #include "soc.h"

    typedef enum
    {
        POINT_OFF = 0,
        POINT_ON = 1
    } ASIO_TubePoint_TypeDef;

    #define ASIO_Key                ((ASIO->KEY) & BIT_0)
    #define ASIO_KeyInt_Enable()    ASIO->KEY |= BIT_1
    #define ASIO_KeyInt_Disable()   ASIO->KEY &= ~BIT_1
    #define ASIO_KeyInt_Clear()     ASIO->KEYINT &= ~BIT_0

    #define ASIO_Switch             ((ASIO->SWITCH) & 0x03FF)
    #define ASIO_Switch0            ((ASIO->SWITCH) & BIT_0)
    #define ASIO_Switch1            ((ASIO->SWITCH) & BIT_1)
    #define ASIO_Switch2            ((ASIO->SWITCH) & BIT_2)
    #define ASIO_Switch3            ((ASIO->SWITCH) & BIT_3)
    #define ASIO_Switch4            ((ASIO->SWITCH) & BIT_4)
    #define ASIO_Switch5            ((ASIO->SWITCH) & BIT_5)
    #define ASIO_Switch6            ((ASIO->SWITCH) & BIT_6)
    #define ASIO_Switch7            ((ASIO->SWITCH) & BIT_7)
    #define ASIO_Switch8            ((ASIO->SWITCH) & BIT_8)
    #define ASIO_Switch9            ((ASIO->SWITCH) & BIT_9)

    #define ASIO_LED_Set(x)         ASIO->LED = x
    #define ASIO_LED0_ON()          ASIO->LED |= BIT_0
    #define ASIO_LED1_ON()          ASIO->LED |= BIT_1
    #define ASIO_LED2_ON()          ASIO->LED |= BIT_2
    #define ASIO_LED3_ON()          ASIO->LED |= BIT_3
    #define ASIO_LED4_ON()          ASIO->LED |= BIT_4
    #define ASIO_LED5_ON()          ASIO->LED |= BIT_5
    #define ASIO_LED6_ON()          ASIO->LED |= BIT_6
    #define ASIO_LED7_ON()          ASIO->LED |= BIT_7
    #define ASIO_LED8_ON()          ASIO->LED |= BIT_8
    #define ASIO_LED9_ON()          ASIO->LED |= BIT_9
    #define ASIO_LED0_OFF()         ASIO->LED &= ~BIT_0
    #define ASIO_LED1_OFF()         ASIO->LED &= ~BIT_1
    #define ASIO_LED2_OFF()         ASIO->LED &= ~BIT_2
    #define ASIO_LED3_OFF()         ASIO->LED &= ~BIT_3
    #define ASIO_LED4_OFF()         ASIO->LED &= ~BIT_4
    #define ASIO_LED5_OFF()         ASIO->LED &= ~BIT_5
    #define ASIO_LED6_OFF()         ASIO->LED &= ~BIT_6
    #define ASIO_LED7_OFF()         ASIO->LED &= ~BIT_7
    #define ASIO_LED8_OFF()         ASIO->LED &= ~BIT_8
    #define ASIO_LED9_OFF()         ASIO->LED &= ~BIT_9

    #define ASIO_TUBEMASK_Set(x)    ASIO->TUBEMASK = x
    #define ASIO_TUBE0_ON()         ASIO->TUBEMASK |= BIT_0
    #define ASIO_TUBE1_ON()         ASIO->TUBEMASK |= BIT_1
    #define ASIO_TUBE2_ON()         ASIO->TUBEMASK |= BIT_2
    #define ASIO_TUBE3_ON()         ASIO->TUBEMASK |= BIT_3
    #define ASIO_TUBE4_ON()         ASIO->TUBEMASK |= BIT_4
    #define ASIO_TUBE5_ON()         ASIO->TUBEMASK |= BIT_5
    #define ASIO_TUBE0_OFF()        ASIO->TUBEMASK &= ~BIT_0
    #define ASIO_TUBE1_OFF()        ASIO->TUBEMASK &= ~BIT_1
    #define ASIO_TUBE2_OFF()        ASIO->TUBEMASK &= ~BIT_2
    #define ASIO_TUBE3_OFF()        ASIO->TUBEMASK &= ~BIT_3
    #define ASIO_TUBE4_OFF()        ASIO->TUBEMASK &= ~BIT_4
    #define ASIO_TUBE5_OFF()        ASIO->TUBEMASK &= ~BIT_5

    void ASIO_Tube_Set(unsigned char tubex, unsigned char data, ASIO_TubePoint_TypeDef POINT);

#endif
