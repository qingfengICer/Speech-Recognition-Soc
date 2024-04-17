
/**************************************************************************//**
 * @file     soc.h
 * @brief    soc 
 *           
 * @version  V1.0
 * @date     2018-4-4
 ******************************************************************************/


#ifndef SOC_H
#define SOC_H

#ifdef __cplusplus
extern "C"	{
#endif

#define SYSTEM_CLOCK   	(25000000U)
#define HCLOCK   				SYSTEM_CLOCK
#define PCLOCK   				SYSTEM_CLOCK


/* -------------------------  Interrupt Number Definition  ------------------------ */
typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                             */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M3 Memory Management Interrupt              */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M3 Bus Fault Interrupt                      */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M3 Usage Fault Interrupt                    */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                       */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                 */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                       */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                   */

 /***  CNSOC CM3 BLE Interrupt Numbers  ****/
	UART0_Over_IRQn							= 4,			/*!< UART0 TX/RX Interrupt                             	*/
	UART0_TXRX_IRQn							= 5,			/*!< UART0 TX/RX over run Interrupt                    	*/
	PDM2PCM_Full_IRQn						= 6,			/*!< PDM2PCM FIFO Full Interrupt                      	*/
	PDM2PCM_Half_IRQn						= 7, 			/*!< PDM2PCM FIFO Half Interrupt                      	*/
	ASIO_KEY_IRQn								= 8, 			/*!< PDM2PCM FIFO Half Interrupt                      	*/
	KEYBOARD_IRQn								= 9				/*!< KEYBOARD Interrupt                             			*/
	
	
} IRQn_Type ;

#include "core_cm3.h"
#include <stdint.h>
//#include <stdio.h>

/* -------------------------  type define  ------------------------ */
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */

/* -------------------------  common macro & enum  ------------------------ */
// standard bits
#define BIT_0		(0x00000001u)
#define BIT_1		(0x00000002u)
#define BIT_2		(0x00000004u)
#define BIT_3		(0x00000008u)
#define BIT_4		(0x00000010u)
#define BIT_5		(0x00000020u)
#define BIT_6		(0x00000040u)
#define BIT_7		(0x00000080u)
#define BIT_8		(0x00000100u)
#define BIT_9		(0x00000200u)
#define BIT_10	(0x00000400u)
#define BIT_11	(0x00000800u)
#define BIT_12	(0x00001000u)
#define BIT_13	(0x00002000u)
#define BIT_14	(0x00004000u)
#define BIT_15	(0x00008000u)
#define BIT_16	(0x00010000u)
#define BIT_17	(0x00020000u)
#define BIT_18	(0x00040000u)
#define BIT_19	(0x00080000u)
#define BIT_20	(0x00100000u)
#define BIT_21	(0x00200000u)
#define BIT_22	(0x00400000u)
#define BIT_23	(0x00800000u)
#define BIT_24	(0x01000000u)
#define BIT_25	(0x02000000u)
#define BIT_26	(0x04000000u)
#define BIT_27	(0x08000000u)
#define BIT_28	(0x10000000u)
#define BIT_29	(0x20000000u)
#define BIT_30	(0x40000000u)
#define BIT_31	(0x80008000u)

// common state
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;


#include "nvic.h"


/*******************************************************************************
 *                 Register Abstraction
 ******************************************************************************/
 /** @addtogroup UDP_CM3_UART - User Define Periferal CM3 UART
  memory mapped structure for Universal Asynchronous Receiver/Transmitter (UART)
  @{
 */
 typedef struct
{
	union {
		__I	uint32_t	RDATA;
		__O	uint32_t	WDATA;
		} DATAR;												/*!< Offset: 0x000  Data Register */                                 
  __IO uint32_t STATR;              /*!< Offset: 0x004  State Register */  
	__IO uint32_t CTRLR;              /*!< Offset: 0x008  Control Register */  		
  __IO uint32_t INTSTATR;           /*!< Offset: 0x00C  Interrupt State and Clear Register */                      
  __IO uint32_t BAUDDIV;            /*!< Offset: 0x010  Baud divider Register */
       uint32_t RESERVED0[243];                                   
  __IO uint32_t IDR[8];             /*!< Offset: 0x3E0  ID register */
       uint32_t RESERVED1[768];                                   
}  UART_Type;    

#define UART_DATAR_MASK										(0x000000ffu)
#define UART_STATR_TX_FULL_MASK						BIT_0
#define UART_STATR_RX_FULL_MASK						BIT_1
#define UART_STATR_TX_OVERRUN_MASK				BIT_2
#define UART_STATR_RX_OVERRUN_MASK				BIT_3
#define UART_CTRLR_TX_EN_MASK							BIT_0
#define UART_CTRLR_RX_EN_MASK							BIT_1
#define UART_CTRLR_TX_INT_MASK						BIT_2
#define UART_CTRLR_RX_INT_MASK						BIT_3
#define UART_CTRLR_TX_ORINT_MASK					BIT_4
#define UART_CTRLR_RX_ORINT_MASK					BIT_5
#define UART_CTRLR_HSM_MASK								BIT_6
#define UART_INTSTATR_TX_MASK							BIT_0
#define UART_INTSTATR_RX_MASK							BIT_1
#define UART_INTSTATR_TXOR_MASK						BIT_2
#define UART_INTSTATR_RXOR_MASK						BIT_3
#define UART_INTSTATR_ALL_MASK						(0x0000000fu)
#define UART_BAUDDIV_MASK									(0x000fffffu)

/*@}*/ /* end of group UDP_CM3_UART */

/** @addtogroup UDP_CM3_PDM2PCM_FIFO - User Define Periferal CM3 PDM2PCM FIFO
  memory mapped structure for PDM2PCM FIFO
  @{
 */
typedef struct
{
	__IO   uint32_t  CTRL;          /*!< Offset: 0x000 Control Register (R/W) */
	__IO   uint32_t  INTR;          /*!< Offset: 0x004 Interrupt Register (R/W) */
	__I    uint32_t  STATE;         /*!< Offset: 0x008 Status Register  (R/W) */
	__I    uint32_t  DATA;          /*!< Offset: 0x00C Data Register    (R/W) */

} PDM2PCM_TypeDef;   

#define PDM2PCM_CTRL_CLR_MASK								BIT_0
#define PDM2PCM_CTRL_EN_MASK								BIT_1
#define PDM2PCM_CTRL_FINT_EN_MASK						BIT_2
#define PDM2PCM_CTRL_HFINT_EN_MASK					BIT_3
#define PDM2PCM_INTR_FULL_MASK							BIT_0
#define PDM2PCM_INTR_HALF_MASK							BIT_1
#define PDM2PCM_STAT_EMPTY_MASK							BIT_0
#define PDM2PCM_STAT_FULL_MASK							BIT_1
#define PDM2PCM_STAT_NUM_MASK								(0x000001fffU << 2)
#define PDM2PCM_DATA_MASK										(0x0000ffffU << 1)


/*@}*/ /* end of group UDP_CM3_FIFO */

/** @addtogroup UDP_CM3_ASIO - User Define Periferal ASIO
  memory mapped structure for ASIO
  @{
 */
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



/*@}*/ /* end of group UDP_CM3_ASIO */


/* Memory mapping of Cortex-M3 UDP Hardware */
#define SRAM_BASE             	(0x00008000UL)		/*!< SRAM Space Base Address */
#define FLASH_BASE           		(0x00000000UL)		/*!< Fash Space Base Address */

#define UART0_BASE            	(0x40004000UL)		/*!< UART0 Base Address */
#define PDM2PCM_BASE            (0x40009000UL)		/*!< PDM2PCM FIFO Base Address */
#define ASIO_BASE	            	(0x40008000UL)		/*!< ASIO Base Address */

#define UART0										((UART_Type *)UART0_BASE)			/*!< UART0 Type Register           */
#define PDM2PCM									((PDM2PCM_TypeDef *)PDM2PCM_BASE)		/*!< PDM2PCM FIFO Type Register           */
#define ASIO										((ASIO_TypeDef *)ASIO_BASE)		/*!< PDM2PCM FIFO Type Register           */

#ifdef __cplusplus
}
#endif

#endif  /* SOC_H */
