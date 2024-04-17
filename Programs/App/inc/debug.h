#ifndef __DEBUG_H
#define __DEBUG_H

#include "soc.h"

#define USE_MCROLIB_MDK

#define DEBUG_BAUDRATE		115200U

void debug_init(void);

void keyboard_init(void);

#endif
