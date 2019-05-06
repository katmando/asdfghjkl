/*
 * DA2C_T1_CTC_IRO_C.c
 *
 * Created: 4/26/2019 7:33:40 PM
 * Author : acate
 */ 


#define F_CPU 16000000UL		// Used to slow delay from 16MHz to 1MHz (delay library)
#include <avr/io.h>				// Standard AVR Library
#include <avr/interrupt.h>		// AVR library containing interrupt functions
#include <util/delay.h>			// AVR library containing _delay_ms() function

#define LED 0b00000100			// Modify LED Bit (Currently: PB2)
#define period 11327			// 'period = (16MHz*0.725s/1024) - 1' for 735ms period
#define duty_cycle 6796			// 'duty_cycle = (16MHz*0.435s/1024) - 1' for 60% Duty Cycle

volatile unsigned char i = 0;	// 8-bit Positive Counter 'i'

int main(void) {
	
	unsigned char p_end = period/256;			// Quotient of Period divided by Counter Size
	unsigned char p_leftover = period%256;		// Remainder of Period divided by Counter Size
	unsigned char dc_end = duty_cycle/256;		// Quotient of Duty Cycle divided by Counter Size
	unsigned char dc_leftover = duty_cycle%256;	// Remainder of Duty Cycle divided by Counter Size
	
	DDRB = LED;											// Set direction of LED (PB2) as an Output
	OCR0A = 0xFF;										// Load Compare Register Value
	TIMSK0 |= (1 << OCIE0A);							// Set Interrupt on Compare Match
	sei();												// Enable Global Interrupts
	TCCR0A = (0<<COM0A1)|(0<<COM0A0);					// Set Compare Output Mode
	TCCR0A = (1<<WGM01)|(0<<WGM00);						// Set WGM to CTC
	TCCR0B = (0<<WGM02)|(1<<CS02)|(0<<CS01)|(1<<CS00);	// Set WGM to CTC (Cont.), Prescaler '1024'
	
	while (1) {
		PORTB |= LED;						// Set Output LED PB2 to 'HIGH'
		i = 0;								// Initialize Counter 'i' to zero
		while (i < p_end) {					// Loop Counter 'i' until Period Quotient is met
			if (i == dc_end) {				// If Counter 'i' is at end of Duty Cycle Quotient...
				while(TCNT0 < dc_leftover);	// Finish Duty Cycle Remainder
				PORTB = 0x00;				// Then set Output LED PB2 to 'LOW'
			}
		}
		while (TCNT0 < p_leftover);			// If Counter 'i' at end of period, then finish remainder
		TCNT0 = 0x00;						// Restart Period and Timer0 from beginning
	}
}

ISR(TIMER0_COMPA_vect) {
	i++;							// Increment Counter 'i'
	return;							// Resume code from where interrupt left off
}
