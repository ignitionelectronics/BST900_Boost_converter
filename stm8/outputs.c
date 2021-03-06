/* Copyright (C) 2015 Baruch Even
 * Modified for PWM fan Control by Derek Jennings 2019
 *
 * This file is part of the B3603 alternative firmware.
 *
 *  B3603 alternative firmware is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  B3603 alternative firmware is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with B3603 alternative firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "outputs.h"
#include "fixedpoint.h"
#include "uart.h"
#include "config.h"

#include "stm8s.h"

#define PWM_VAL 0x2000	//Gives PWM frequency of 1.591kHz
#define PWM_HIGH (PWM_VAL >> 8)
#define PWM_LOW (PWM_VAL & 0xFF)

#ifdef __GNUC__
#define INLINE
#else
#define INLINE inline
#endif

extern state_t state;

void pwm_init(void)
{
	/* Timer 1 Channel 1 for Iout control */
	TIM1_CR1 = 0x10; // Down direction
	TIM1_ARRH = PWM_HIGH; // Reload counter = 8192
	TIM1_ARRL = PWM_LOW;
	TIM1_PSCRH = 0; // Prescaler 0 means division by 1
	TIM1_PSCRL = 0;
	TIM1_RCR = 0; // Continuous

	TIM1_CCMR1 = 0x70;    //  Set up to use PWM mode 2.
	TIM1_CCER1 = 0x03;    //  Output is enabled for channel 1, active low
	TIM1_CCR1H = 0x00;      //  Start with the PWM signal off
	TIM1_CCR1L = 0x00;
	
	/* Timer 1 Channel 3 for Fan Control */
	TIM1_CCMR3 = 0x70;    //  Set up to use PWM mode 2.
	TIM1_CCER2 = 0x03;    //  Output is enabled for channel 3, active low
	TIM1_CCR3H = 0x10;      //  Start with the PWM signal on to prove the fan works
	TIM1_CCR3L = 0x00;	
	
	TIM1_BKR = 0x80;       //  Enable the main output.
	
	/* Timer 2 Channel 1 for Vout control */
	TIM2_ARRH = PWM_HIGH; // Reload counter = 8192
	TIM2_ARRL = PWM_LOW;
	TIM2_PSCR = 0; // Prescaler 0 means division by 1
	TIM2_CR1 = 0x00;

	TIM2_CCMR1 = 0x70;    //  Set up to use PWM mode 2.
	TIM2_CCER1 = 0x03;    //  Output is enabled for channel 1, active low
	TIM2_CCR1H = 0x00;      //  Start with the PWM signal off
	TIM2_CCR1L = 0x00;

	// Timers are still off, will be turned on when output is turned on
}

INLINE void cvcc_led_cc(void)
{
	PA_ODR |= (1<<3);
	PA_DDR |= (1<<3);
}

INLINE void cvcc_led_cv(void)
{
	PA_ODR &= ~(1<<3);
	PA_DDR |= (1<<3);
}

INLINE void cvcc_led_off(void)
{
	PA_DDR &= ~(1<<3);
}

uint16_t pwm_from_set(uint16_t set, calibrate_t *cal)
{
	uint32_t tmp;

    // calc   scalar * fixedpoint
    //
	// 'a' is in fixed point format
    // 'set' is in scalar format
    //  ->  a * set  is also in fixed point format without adjustment.
	tmp = (uint32_t)set * cal->a;

	// calc x*a + b
    // tmp is in fixed point format
    // 'b' is in fixed point format
    // ->  tmp + b  is also in fixed point format without adjustment.
	tmp += cal->b;

	// PWM is 0x8000 and as such amounts to a shift by 13 so to multiple by PWM
	// we simply shift all calculations by 3 and this avoids overflows and loss
	// of precision.

    // return value rounded to the nearest integer 
	return fixed_round(tmp);
}

INLINE void control_voltage(cfg_output_t *cfg, cfg_system_t *sys)
{
	uint16_t ctr = pwm_from_set(cfg->vset, &sys->vout_pwm);
	uart_write_str("PWM VOLTAGE ");
    uart_write_centivalue(cfg->vset);
	uart_write_crlf();

	TIM2_CCR1H = ctr >> 8;
	TIM2_CCR1L = ctr & 0xFF;
	TIM2_CR1 |= 0x01; // Enable timer
}
INLINE void control_current(cfg_output_t *cfg)
{
	uart_write_str("PWM CURRENT ");
    uart_write_millivalue(cfg->cset);
	uart_write_crlf();
// In closed loop let current slide from one setting to the next
	TIM1_CR1 |= 0x01; // Enable timer
}

INLINE void control_fan()
{
	uint16_t ctr = 0;
#ifdef FAN_PWM
	if (state.cout > MIN_FAN_CURRENT) ctr = state.cout+256;	// Full speed will be at 7.936A
#else
	if (state.cout > MIN_FAN_CURRENT) ctr = 0x2000;
#endif
	TIM1_CCR3H = ctr >> 8;
	TIM1_CCR3L = ctr & 0xff;
	TIM1_CR1 |= 0x01; // Enable timer
}

void output_commit(cfg_output_t *cfg, cfg_system_t *sys, uint8_t state_constant_current)
{
	// Startup and shutdown orders need to be in reverse order
	if (sys->output) {
		control_voltage(cfg, sys);
		control_current(cfg);	

		// We turned on the PWMs above already
		PB_ODR &= ~(1<<4);
		output_check_state(sys, state_constant_current);
	} else {
		// Set Output Enable OFF
		PB_ODR |= (1<<4);

		// Turn off PWM for Iout
		TIM1_CCR1H = 0;
		TIM1_CCR1L = 0;
		TIM1_CR1 &= 0xFE; // Disable timer

		// Turn off PWM for Vout
		TIM2_CCR1H = 0;
		TIM2_CCR1L = 0;
		TIM2_CR1 &= 0xFE; // Disable timer

		// Turn off CV/CC led
		cvcc_led_off();
	}
}

void output_check_state(cfg_system_t *sys, uint8_t state_constant_current)
{
	if (sys->output) {
		if (state_constant_current)
			cvcc_led_cc();
		else
			cvcc_led_cv();
	}
}
