Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


Heap_Size       EQU     0x00000400

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


; Vector Table Mapped to Address 0 at Reset

                PRESERVE8
                THUMB

                AREA    RESET, DATA, READONLY
				EXPORT  __Vectors

__Vectors       DCD     __initial_sp  	; Top of Stack
                DCD     Reset_Handler  	; Reset Handler
                DCD     0
                DCD     0
                DCD     0
                DCD     0
                DCD     0
                DCD     0
                DCD     0
                DCD     0
                DCD     0
                DCD     0
                DCD     0
                DCD     0
                DCD     0
                DCD     SysTick_Handler		; SysTick_Handler

                ; External Interrupts
				;DCD     KEY_Handler			; 9 IRQ9 Handler,Keyboard
                DCD     0         			; 0
				DCD     0      				; 1
                DCD     0      				; 2
                DCD     0	   				; 3
                DCD     UART0_Over_Handler	; 4 UART0 TX/RX/OverRun 
                DCD     UART0_TXRX_Handler	; 5 UART0 TX/RX/OverRun    
                DCD     FIFO_Full_Handler	; 6 FIFO full Handler					
                DCD     FIFO_Half_Handler	; 7 FIFO half full Handler
                DCD     KEY_IRQHandler		; 8 ASIO KEY Handler 
                DCD     KEY_Handler			; 9 IRQ9 Handler,Keyboard
                ;DCD     LCDINI_Handler		; 10 LCD ini
                DCD     0					; 11
                DCD     0					; 12
                DCD     0					; 13
                DCD     0					; 14
                DCD     0					; 15
__Vectors_End

                AREA    |.text|, CODE, READONLY

; Reset Handler
Reset_Handler   PROC
				EXPORT  Reset_Handler
;                ENTRY
;                B       .
				IMPORT  __main
                LDR     R0, =__main
                BX      R0                  ; Branch to __main
                ENDP

SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
				ENDP

Default_Handler PROC

				EXPORT	UART0_Over_Handler		[WEAK]
				EXPORT	UART0_TXRX_Handler		[WEAK]
				EXPORT	FIFO_Full_Handler		[WEAK]
				EXPORT	FIFO_Half_Handler		[WEAK]
				EXPORT	KEY_IRQHandler			[WEAK]
				EXPORT	KEY_Handler				[WEAK]
				;EXPORT	LCDINI_Handler				[WEAK]

UART0_Over_Handler
UART0_TXRX_Handler
FIFO_Full_Handler
FIFO_Half_Handler
KEY_IRQHandler
KEY_Handler
;LCDINI_Handler
				B		.
				
				ENDP				

                ALIGN   4                   ; Align to a word boundary


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

				EXPORT  __initial_sp
				EXPORT  __heap_base
				EXPORT  __heap_limit

                ELSE

				IMPORT  __use_two_region_memory
				EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem+  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


				END
