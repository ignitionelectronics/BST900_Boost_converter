#ifndef MAIN_H
#define MAIN_H

#define BST900		//Uncomment if building for BST400 boost converters

#define CRLF	"\r\n"	//Use   \n or  \r\n 

#include "display.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef BST900
#define MODEL "BST400"


#define CAP_VMIN 800 // 10V --centiVolts
#define CAP_VMAX 12000 // 80 V --centiVolts
#define CAP_VSTEP 10 // 10cV

#define CAP_CMIN 100 // 100 mA
#define CAP_CMAX 10000 // 15 A
#define CAP_CSTEP 10 // 10 mA

#else
#define MODEL "BST900"

#define CAP_VMIN 1000 // 10V	--centiVolts
#define CAP_VMAX 12000 // 120 V	--centiVolts
#define CAP_VSTEP 10 // 10cV

#define CAP_CMIN 10 // 10 mA
#define CAP_CMAX 15000 // 15 A
#define CAP_CSTEP 10 // 10 mA
#endif //BST900
#define FW_VERSION "1.0.0"

//#define FAN_PWM					//If defined Fan will use PWM which can cause the fan windings to 'sing'
								// If this is annoying leave undefined and Fan will be at max speed above MIN_FAN_CURRENT
#define MIN_FAN_CURRENT	2500	//Output current in mA below which fan is stopped


bool set_output(const char *s);
bool set_voltage(uint16_t voltage);
bool set_current(uint16_t current);

#endif //MAIN_H
