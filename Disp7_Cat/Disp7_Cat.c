/*
 * CFile1.c
 *
 * Created: 1/23/2025 7:07:50 PM
 *  Author: diego
 */ 
#include "Disp7_Cat.h"

uint8_t tabla_7_seg[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0xF1};

void initDisplay(void){
	DDRD |= 0xFC;
	PORTD |= 0xFC;
	DDRB |= 0x03; // 0b0011_1111
	PORTB |= 0x03; //0b0000_0011
}

void display(uint16_t numero){
	uint16_t valor_para_display = ~tabla_7_seg[numero];
	uint8_t valor_portd = 0;
	uint8_t valor_portb = 0;
	valor_para_display <<= 2;
	valor_portd = ~(valor_portd | valor_para_display);
	valor_para_display >>= 8;
	valor_portb = (~(valor_portb | valor_para_display) & 0x03); // 0b0000_0011
	PORTB = (PORTB & 0xFC) | valor_portb; // 0b1111_1100
	PORTD = valor_portd;
}