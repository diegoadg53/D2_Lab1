/*
 * D2_Lab1.c
 *
 * Created: 1/23/2025 7:05:55 PM
 * Author : diego
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "Disp7_Cat/Disp7_Cat.h"

uint16_t valor_contador = 0;
uint8_t alternador_displays = 1;
uint8_t Conteo_Inicio_Carrera = 0x03;
uint8_t Punteo_J1 = 0;
uint8_t Punteo_J2 = 0;
uint8_t v_portb = 0;
uint8_t v_portd = 0;
uint8_t disp_J2 = 0;
uint8_t J1_Apachado = 0;
uint8_t J2_Apachado = 0;

void setup(void);

int main(void)
{
    setup();
    while (1) 
    {
		if (Conteo_Inicio_Carrera == 0x01) {
			cli();
			PORTB &= ~(1 << PINB3);
			PORTB |= (1 << PINB2);
			display(5);
			_delay_ms(1000);
			display(4);
			_delay_ms(1000);
			display(3);
			_delay_ms(1000);
			display(2);
			_delay_ms(1000);
			display(1);
			_delay_ms(1000);
			display(0);
			Conteo_Inicio_Carrera = 0x02;
			sei();
		}
		else
		{
			if (alternador_displays == 0)
			{
				PORTB &= ~(1 << PINB3);
				PORTB |= (1 << PINB2);
				if (Conteo_Inicio_Carrera == 0x03)
				{
					display(8);
				} else {
					display(0);
				}
			} 
			else if (alternador_displays == 1)
			{
				PORTB &= ~(1 << PINB2);
				PORTB |= (1 << PINB3);
				if ((Punteo_J1!=5)&(Punteo_J2!=5)) {
				v_portb = 0;
				v_portd = ((0x02 << Punteo_J1)&0x3C);
				disp_J2 = 0x01 << (Punteo_J2 + 5);
				v_portd |= (disp_J2 & 0xC0);
				disp_J2 >>= 8;
				v_portb = disp_J2 & 0x03;
				PORTD = v_portd;
				PORTB = (PORTB & 0xFC) | v_portb;
				} else if (Punteo_J1 == 5)
				{
					PORTD = (PORTD&0x00)|0x3C;
					PORTB &= 0xFC;
				} else if (Punteo_J2 == 5)
				{
					PORTD = (PORTD&0x00)|0xC0;
					PORTB |= 0x03;
				}
			}
		}
		
    }
}

void setup(void){
	cli();
	initDisplay();
	DDRB |= 0x0C;
	PORTB |= 0x0C; //0b0000_1100
	DDRC = 0x00;
	PCMSK1 |= 0x07;
	PCICR |= (1 << PCIE1);
	
	TCCR0B |= (1 << CS00)|(1<<CS02);
	TCNT0 = 177;
	
	TIMSK0 = (1 << TOIE0);
	sei();
}

ISR(PCINT1_vect){
	if (!(PINC & (1 << PINC0))) // J2
	{
		if (J2_Apachado == 0)
		{
		if (Conteo_Inicio_Carrera == 0x02)
		{
			Punteo_J2 += 1;
			if (Punteo_J2 == 4) {
				Conteo_Inicio_Carrera = 0x03;
				Punteo_J2 = 5;
				Punteo_J1 = 0;
			}
		}
		}
		J2_Apachado = 1;
	}
	else {
		J2_Apachado = 0;
	}
	if (!(PINC & (1 << PINC1))) // J1
	{
		if (J1_Apachado == 0)
		{
		if (Conteo_Inicio_Carrera == 0x02)
		{
			Punteo_J1 += 1;
			if (Punteo_J1 == 4) {
				Conteo_Inicio_Carrera = 0x03;
				Punteo_J2 = 0;
				Punteo_J1 = 5;
			}
		}
		}
		J1_Apachado = 1;
	}
	else {
		J1_Apachado = 0;
	}
	if (!(PINC & (1 << PINC2))) // Iniciar
	{
		if (Conteo_Inicio_Carrera == 0x03)
		{
			Conteo_Inicio_Carrera = 0x01;
			Punteo_J1 = 0;
			Punteo_J2 = 0;
		}
	}
}

ISR(TIMER0_OVF_vect){
	TCNT0 = 177;
	alternador_displays += 1;
	if (alternador_displays == 2)
	{
		alternador_displays = 0;
	}
}