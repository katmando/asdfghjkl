/*
 * DA2A_T1_C.c
 *
 * Created: 5/6/2019 4:28:45 PM
 * Author : acate
 */ 



#define F_CPU 16000000		// Slow Rate of delay from 16MHz to 1MHz
#include <avr/io.h>			// Standard AVR Library
#include <util/delay.h>		// AVR library containing _delay_ms() function

#define LED 0b00000100		// Modify LED Bit here (Currently PB2)

int main(void) {			// Main Function
	DDRB = LED;				// Set PB2 as an Output
	PORTB = LED;			// Set PB2 as 'High'

	while (1) {				// Loop LED Toggle, Period of 735ms (60% High, 40% Low)
		_delay_ms(435);		// Delay Program by 435ms (Multiply '16' because 16MHz)
		PORTB &= ~LED;		// Turn OFF LED (High-to-Low)
		_delay_ms(290);		// Delay Program by 290ms (Multiply '16' because 16MHz)
		PORTB |= LED;		// Turn ON LED (Low-to-High)
	}
}