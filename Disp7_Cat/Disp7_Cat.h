/*
 * Disp7_Cat.h
 *
 * Created: 1/23/2025 7:55:42 PM
 *  Author: diego
 */ 


#ifndef DISP7_CAT_H_
#define DISP7_CAT_H_


#include <stdint.h>
#include <avr/io.h>

void initDisplay(void);

void display(uint16_t numero);


#endif /* DISP7_CAT_H_ */