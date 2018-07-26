#ifndef MAIN_H
#define MAIN_H

#include "display.h"

#define NULL ( (void *) 0)

void set_output(uint8_t *s);
void set_voltage(uint8_t *s, uint16_t voltage);
void set_current(uint8_t *s, uint16_t current);

#endif //MAIN_H
