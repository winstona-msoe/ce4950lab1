/*
 * testArduinoUNO.c
 * "Blink" program for testing functionality of the UNO.
 * Onboard LED ("L") should flash on and off.
 *
 *  Created on: Jul 15, 2011
 *      Author: Cory J. Prust, Ph.D.
 */

#include <avr/io.h>
#include <inttypes.h>

void delay(uint16_t x);

int main(void)
{
	DDRB |= (1<<PORTB5);
	PORTB = 0;

	while(1)
	{
		delay(5);
		PORTB ^= (1<<PORTB5);
	}
}

void delay(uint16_t x)
{
	uint16_t i,j;

	for(i=0;i<x;i++)
		for(j=0;j<10000;j++)
			;
	return;
}
