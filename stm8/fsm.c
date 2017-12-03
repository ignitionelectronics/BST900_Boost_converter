
// Finite State Enginer for menu structure

#include <stdint.h>
//#include "stm8s.h"
#include "display.h"
#include "buttons.h"
#include "config.h"
#include "fsm.h"

// repeated integer calculations to char 
#define D4	10000
#define D3	1000
#define D2	100
#define D1	10


uint8_t	fsm_state=FSM_SHOWVIN;	// default start to show VIN... for now
extern state_t state;


uint8_t uint16_to_char(uint16_t value, uint16_t devider)
{
return '0' + (value / devider) % 10;
}


void process_fsm_state(uint8_t button) 
{
// all menu state/actions and transistions are done here

switch (fsm_state) {
    case FSM_SHOWVIN:

        if (button == BUTTON_SET) {
            fsm_state=FSM_SHOWVOUT;
        }
    
            // set/keep/update VIN as output
            {
                uint8_t ch1;
                uint8_t ch2;
                uint8_t ch3;
                uint8_t ch4;

                //ch1 = '0' + (state.vin / 10000) % 10;
                ch1 = uint16_to_char(state.vin, D4);

                //ch2 = '0' + (state.vin / 1000) % 10;
                ch2 = uint16_to_char(state.vin, D3);

                //ch3 = '0' + (state.vin / 100) % 10;
                ch3 = uint16_to_char(state.vin, D2);
                
                //ch4 = '0' + (state.vin / 10 ) % 10;
                ch4 = uint16_to_char(state.vin, D1);

                display_show(ch1, 0, ch2, 1, ch3, 0, ch4, 0);
            }

            
            // state transitions
        break;
    case FSM_SHOWVOUT:
        if (button == BUTTON_SET) {
            fsm_state=FSM_SHOWVIN;
        }
    
            {
                uint8_t ch1;
                uint8_t ch2;
                uint8_t ch3;
                uint8_t ch4;

                //ch1 = '0' + (state.vout / 10000) % 10;
                ch1 = uint16_to_char(state.vout, D4);

                //ch2 = '0' + (state.vout / 1000) % 10;
                ch2 = uint16_to_char(state.vout, D3);

                //ch3 = '0' + (state.vout / 100) % 10;
                ch3 = uint16_to_char(state.vout, D2);
                
                //ch4 = '0' + (state.vout / 10 ) % 10;
                ch4 = uint16_to_char(state.vout, D1);

                display_show(ch1, 0, ch2, 1, ch3, 0, ch4, 0);
            }

        break;
}


}
