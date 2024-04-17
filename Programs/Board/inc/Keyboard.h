#ifndef __Keyboard_H__
#define __Keyboard_H__

#include <stdint.h>
#include<stdbool.h>

//Keyboard DEF
#define Keyboard_BASE 0x40003000
#define KeyboardReg (*(uint32_t *)Keyboard_BASE)

// Keyboard int
void KEY_Handler(void);

#endif
