#ifndef MAIN_H
#define MAIN_H

#define BST900		//Uncomment if building for BST400 boost converters
//#define CLOSED_LOOP_CC	//Comment to disable experimental Closed Lop Constant Current Control

#define CRLF	"\r\n"	//Use \n for Linux \r\n for Windows

#include "display.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef BST900
#define MODEL "BST400"

/*There is a problem. BST900 supports output voltages up to 120V which exceeds the addressing 
 * range of a 16 bit integer.
 * Either output voltage is limited to 65000mV or else all the code
 * has to be converted to uint32_t which could bust memory constraints.
 * Other alternative is to work with 10mV resolution.
* 
* */


#define CAP_VMIN 8000 // 10V
#define CAP_VMAX 65000 // 80 V
#define CAP_VSTEP 10 // 10mV

#define CAP_CMIN 100 // 100 mA
#define CAP_CMAX 10000 // 15 A
#define CAP_CSTEP 10 // 10 mA

#else
#define MODEL "BST900"

#define CAP_VMIN 10000 // 10V
#define CAP_VMAX 65500 // 120 V
#define CAP_VSTEP 10 // 10mV

#define CAP_CMIN 10 // 10 mA
#define CAP_CMAX 15000 // 15 A
#define CAP_CSTEP 10 // 10 mA
#endif //BST900
#define FW_VERSION "0.0.1"
#define MAX_FAN_CURRENT	8000	//Output current in mA above which fan is at full speed
#define MIN_FAN_CURRENT	2000	//Output current in mA below which fan is stopped

bool set_output(const char *s);
bool set_voltage(uint16_t voltage);
bool set_current(uint16_t current);

#endif //MAIN_H
