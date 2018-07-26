#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

//Bit wise enum
typedef enum {
	BUTTON_NONE = 0x0,
	BUTTON_SET  = 0x1,
	BUTTON_DOWN = 0x2,
	BUTTON_UP   = 0x4,
	BUTTON_OK   = 0x8
} button_t;

uint8_t read_buttons(void);

#endif //BUTTONS_H
