/*
 * UART0.c
 *
 * Created: 12/10/2022 3:33:56 PM
 *  Author: snowg
 */ 
#include <avr/io.h>
#include "UART0.h"

void UART0_init(void)
{
	UBRR0H = 0x00;			// 9600 ??? ??
	UBRR0L = 207;
	UCSR0A |= 1<<U2X0;			// 2?? ??
	// ???, 8?? ???, ??? ??, 1?? ?? ?? ??
	UCSR0C |= 0x06;
	
	UCSR0B |= 1<<RXEN0;		// ??? ??
	UCSR0B |= 1<<TXEN0;
}

void UART0_transmit(char data)
{
	while( !(UCSR0A & (1 << UDRE0)) );	// ?? ?? ??
	UDR0 = data;				// ??? ??
}

unsigned char UART0_receive(void)
{
	while( !(UCSR0A & (1<<RXC0)) );	// ??? ?? ??
	return UDR0;
}

void UART0_print_string(char *str)	// ??? ??
{
	for(int i = 0; str[i]; i++)			// ‘\0’ ??? ?? ??? ??
	UART0_transmit(str[i]);			// ??? ?? ??
}

void UART0_print_1_byte_number(uint8_t n)
{
	char numString[4] = "0";
	int i, index = 0;
	
	if(n > 0){					// ??? ??
		for(i = 0; n != 0 ; i++)
		{
			numString[i] = n % 10 + '0';
			n = n / 10;
		}
		numString[i] = '\0';
		index = i - 1;
	}
	
	for(i = index; i >= 0; i--)		// ??? ???? ???? ??
	UART0_transmit(numString[i]);
}