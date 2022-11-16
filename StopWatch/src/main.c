/*
 * main.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Ahmed Abdelaal
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
int clk_count = 0, paused, startFlag;
int sec1 = 0, sec2 = 0, min1 = 0, min2 = 0;
ISR(TIMER0_COMP_vect) {
	++clk_count;
	if (clk_count == 4) {
		clk_count = 0;
		++sec1;
		if (sec1 == 10) {
			sec1 = 0;
			++sec2;
		}
		if (sec2 == 6) {
			sec2 = 0;
			++min1;
		}
		if (min1 == 10) {
			min1 = 0;
			++min2;
		}
		if (sec2 == 10) {
			min2 = 0;
		}
	}
}
ISR(INT0_vect) {
	TCCR0 ^= ((1 << CS02) | (1 << CS00));
	if (startFlag)
		startFlag = 0;
	else
		paused = ~paused;
}
ISR(INT1_vect) {
	TCCR0 &= ~((1 << CS02) | (1 << CS00));
	TCNT0 = 0;
	sec1 = 0;
	sec2 = 0;
	min1 = 0;
	min2 = 0;
}
int main() {
	DDRC |= 0xFF;
	SREG |= (1 << 7);
	GICR |= (1 << INT0) | (1 << INT1);
	MCUCR |= (1 << ISC00) | (1 << ISC01) | (1 << ISC10) | (1 << ISC11);
	TCCR0 |= (1 << FOC0) | (1 << WGM01);
	TCCR0 &= ~((1 << CS02) | (1 << CS00));
	TCNT0 = 0;
	OCR0 = 244;
	TIMSK |= (1 << OCIE0);
	PORTC = 0xF0;
	paused = 0;
	startFlag=1;
	unsigned char seg1, seg2, seg3, seg4;
	while (1) {
		if (paused) {
			PORTC &= 0x0F;
			_delay_ms(400);
		}

		seg1 = (PORTC & 0x00) | (sec1 & 0x0F) | 0x10;
		PORTC = seg1;
		_delay_ms(0.005);
		//==========================
		seg2 = (PORTC & 0x00) | (sec2 & 0x0F) | 0x20;
		PORTC = seg2;
		_delay_ms(0.005);
		//==========================
		seg3 = (PORTC & 0x00) | (min1 & 0x0F) | 0x40;
		PORTC = seg3;
		_delay_ms(0.005);
		//==========================
		seg4 = (PORTC & 0x00) | (min2 & 0x0F) | 0x80;
		PORTC = seg4;
		_delay_ms(0.005);


	}
	return 0;
}
