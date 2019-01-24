#ifndef MAIN_H
#define MAIN_H

#define BST900		//Uncomment if building for BST400 boost converters

#include "display.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef NULL
#define NULL ((void *) 0)
#endif


#ifndef BST900
#define FW_VERSION "1.0.0"
#define MODEL "BST400"

#define CAP_VMIN 8000 // 10mV
#define CAP_VMAX 80000 // 80 V
#define CAP_VSTEP 10 // 10mV

#define CAP_CMIN 1 // 1 mA
#define CAP_CMAX 10000 // 10 A
#define CAP_CSTEP 10 // 1 mA

#else
#define FW_VERSION "1.0.0"
#define MODEL "BST900"

#define CAP_VMIN 8000 // 10mV
#define CAP_VMAX 120000 // 120 V
#define CAP_VSTEP 10 // 10mV

#define CAP_CMIN 1 // 1 mA
#define CAP_CMAX 15000 // 15 A
#define CAP_CSTEP 10 // 10 mA
#endif //BST900


bool set_output(const char *s);
bool set_voltage(uint16_t voltage);
bool set_current(uint16_t current);

#endif //MAIN_H
