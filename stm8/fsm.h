#ifndef FSM_H
#define FSM_H

#include "config.h"
#include "display.h"

typedef enum {
	FSM_DISP_VIN,
	FSM_DISP_VOUT,
	FSM_DISP_IOUT,
	FSM_VOUT_CHANGE,
	FSM_IOUT_CHANGE,
	FSM_DISP_CONF,
	FSM_OUT_ENABLE,
	FSM_OUT_DISABLE,
	FSM_DISP_SAVE,
	FSM_SAVE_CFG
} fsm_states_t;

typedef union {
    struct {
        unsigned char is_button_set: 1;
	unsigned char is_button_down: 1;
	unsigned char is_button_up: 1;
        unsigned char is_button_ok: 1;
        unsigned char is_button_none: 1;
        unsigned char is_output: 1;
        unsigned char is_save: 1;
    } st;
    unsigned char event_flag;
} fsm_event_t;

void process_fsm(button_t button, cfg_system_t *sys, cfg_output_t *cfg, state_t *stt); 

#endif //FSM_H
