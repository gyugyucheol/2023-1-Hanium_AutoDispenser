#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#include "TC_config.h"

int main(void)
{
	DDRA |= (1 << PORTA0);
	
	DDRC |= (1 << PORTC0) | (1 << PORTC1);//PORTC0 : EN, PORTC1 : CW;
	DDRC |= (1 << PORTC4) | (1 << PORTC5);//PORTC4 : EN, PORTC5 : CW;

	PORTC &= ~(1 << PORTC0);//PORTC2 low : enable motor driver
	PORTC &= ~(1 << PORTC4);//PORTC4 low : enable motor driver
	
	InitializeTimer0();
	InitializeTimer1();
	InitializeTimer2();//PTB7을 OC2 핀으로 사용 OC1C 핀과 겹칩 주의
	InitializeTimer3();
	_delay_ms(5000);
	
	OC0PinStart();
	OC2PinStart();
	while(1)
	{ 
		PORTC |= (1 << PORTC1);//PORTC1 high : CCW : leftmove
		OCR1A = ICR1VALUE/2;
		
		PORTC |= (1 << PORTC5);//PORTC3 high : CCW : down
		OC2PinStart();
		
		PORTA &= ~(1 << PORTA0);
		
		_delay_ms(1000);
		
		OC2PinStop();
		OCR1A = ICR1VALUE;
		_delay_ms(1000);
		
		PORTC &= ~(1 << PORTC1);//PORTC1 low : CW :rightmove
		OCR1A = ICR1VALUE/2;
		
		PORTC &= ~(1 << PORTC5);//PORTC3 low : CW : up
		OC2PinStart();
		PORTA |= (1 << PORTA0);
		_delay_ms(1000);
		
		OC2PinStop();
		OCR1A = ICR1VALUE;
		_delay_ms(1000);
	}

	return 0;
}