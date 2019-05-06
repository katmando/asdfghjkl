/*
 * DA2A_T1_C.c
 *
 * Created: 4/27/2019 12:41:00 PM
 * Author : acate
 */ 




#define F_CPU 16000000UL	// Slow rate of delay from 16MHz to 1MHz
#include <avr/io.h>			// Standard AVR Library
#include <util/delay.h>		// AVR library containing _delay_ms() function

#define LED 0b00000100		// Modify LED bit here (PB2 in program)
#define SWITCH 0b00000100	// Modify SWITCH bit here (PC2 in program)

int main(void) {			// Main Function
	DDRC = (0<<2);			// Set/Mask PC2 as an Input
	PORTC = (1<<2);			// Activate Pull-up on PC2 (resistor connected to VCC)
	DDRB = (1<<2);			// Set/Mask PB2 as an Output
	PORTB = (0<<2);			// Set/Mask PB2 as 'Low'
	
	while (1) {								// Poll: Check to read if PC2 is 'High'
		if ((~PINC & SWITCH) == SWITCH){	// If 'High' Turn LED ON for 1250ms, Else Loop
			PORTB |= LED;					// Set/Mask PB2 as 'High'
			_delay_ms(1250);				// Delay Program by 1250ms
			PORTB &= ~LED;					// Set/Mask PB2 as 'Low'/End Program
		}
	}
}