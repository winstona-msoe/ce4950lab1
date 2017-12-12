#ifndef _UARTPRINT_H
#define _UARTPRINT_H

#include <avr/io.h>
#include <stdio.h>
#define BAUD_RATE 57600

void uart_init(void) {
	UBRR0H = (((F_CPU / BAUD_RATE) / 16) - 1) >> 8; // set baud rate
	UBRR0L = (((F_CPU / BAUD_RATE) / 16) - 1);
	UCSR0A = 0;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // enable Rx & Tx
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // config USART; 8N1
}

void uart_flush(void) {
	unsigned char dummy;
	while (UCSR0A & (1 << RXC0))
		dummy = UDR0;
}

int uart_putch(char ch, FILE *stream) {
	if (ch == '\n')
		uart_putch('\r', stream);
	while (!(UCSR0A & (1 << UDRE0)))
		;
	UDR0 = ch;

	return 0;
}

int uart_getch(FILE *stream) {
	unsigned char ch;
	while (!(UCSR0A & (1 << RXC0)))
		;
	ch = UDR0;
	/* Echo the Output Back to terminal */
	uart_putch(ch, stream);
	return ch;
}

void ansi_cl(void) {
	// ANSI clear screen: cl=\E[H\E[J
	putchar(27);
	putchar('[');
	putchar('H');
	putchar(27);
	putchar('[');
	putchar('J');
}

void ansi_me(void) {
	// ANSI turn off all attribute: me=\E[0m
	putchar(27);
	putchar('[');
	putchar('0');
	putchar('m');
}

void ansi_cm(unsigned char row, unsigned char col) {
	// ANSI cursor movement: cl=\E%row;%colH
	putchar(27);
	putchar('[');
	printf("%d", row);
	putchar(';');
	printf("%d", col);
	putchar('H');
}

#endif
