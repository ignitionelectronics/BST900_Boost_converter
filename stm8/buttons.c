#include <stdint.h>

#include "uart.h"
#include "stm8s.h"
#include "buttons.h"


button_t debounce(button_t raw_state)
{
	static uint16_t count = 0, active = 0;
	static button_t eval_state = BUTTON_SET;
	
	if ((raw_state == eval_state) && (count < 50))
		count++;
	if ((raw_state == BUTTON_NONE) && (count > 0))
		count--;
	
	//Reset if a new button is pressed
	if ((raw_state != BUTTON_NONE) && (raw_state != eval_state)) {
		count = 0;
		active = 0;
		eval_state = raw_state;
		return BUTTON_NONE;
	}
	
	//reached the upper point, return button state
	if (count == 50) {
		eval_state = raw_state;
		active = 1;
		return eval_state;
	}
	
	//reached the downer point, return button None
	if (count == 0) {
		active = 0;
		return BUTTON_NONE;
	}
	
	//when going down, return last state till count = 0
	if (active)
		return eval_state;
	
	return BUTTON_NONE;
}

uint8_t read_buttons(void)
{
	button_t button = BUTTON_NONE;
        uint8_t i;

	// PD0
	if ((PD_IDR & (1<<1)) == 0)
		button = BUTTON_DOWN;

	// PC7
	if ((PC_IDR & (1<<7)) == 0)
		button = BUTTON_SET;

	//only possible to scan for secondary keys when dominant keys are not pressed. timing wise this is not fully monkey proof, but will work.
	if (!button) {
		// set PD1 to output mode
		PD_ODR &= ~(1<<1);		// Output value... ~(1<<1) == 1111 1101 set PD1 output to 0
		PD_DDR |= 1<<1;			// set PD1 to output (set to 1)
		PD_CR1 |= 1<<1;			// Push Pull (set to 1)
		//PD_CR2 &= ~(1<<1);		// disable interrupts
		//PD_CR2 |= 1<<1;		// 10 Mhz

		// read PC7
		if ((PC_IDR & (1<<7)) == 0)
			button = BUTTON_OK;

		// Restore to inputs
		//PD_OD |= ~(1<<1);             // ~(1<<1) == 1111 1101 set PD1 output to 0
		PD_DDR &= ~(1<<1);              // set PD1 to input (set to 0)
		PD_CR1 |= 1<<1;                 // Set PD1 to Input/pullup (set to 1)

 		for (i=0; i<30; i++)__asm__("nop\n");	// wait for slow recovery/raise on pull-up, may reduced to maybe 10, but gets more sesetive to erros
	}

	if (!button) {
		// set PC7 to output mode
		PC_ODR &= ~(1<<7);		// Output value... ~(1<<1) == 0111 1111 set PC7 output to 0
		PC_DDR |= 1<<7;			// set PC7 to output (set to 1)
		PC_CR1 |= 1<<7;			// Push Pull (set to 1)

		// PD1    
		if ((PD_IDR & (1<<1)) == 0)
			button=BUTTON_UP;

		// Restore to inputs
		//PD_OD |= ~(1<<1);             // ~(1<<1) == 1111 1101 set PC7 output to 0
		PC_DDR &= ~(1<<7);              // set PC7 to input (set to 0)
		PC_CR1 |= 1<<7;                 // set PC7 to Input/pullup (set to 1)
	}

	//debounce
	button = debounce(button);
	return button;
}

