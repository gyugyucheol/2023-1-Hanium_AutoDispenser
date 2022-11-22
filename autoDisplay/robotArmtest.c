#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#include "TC_config.h"
#include "servoMove.h"
#include "stepMove.h"



int main(void)
{
	DDRA |= (1 << PORTA0);
	
	DDRC |= (1 << PORTC0) | (1 << PORTC1);//vertical, PORTC0 : EN, PORTC1 : CW;
	DDRC |= (1 << PORTC4) | (1 << PORTC5);//horizontal, PORTC4 : EN, PORTC5 : CW;

	PORTC &= ~(1 << PORTC0);//PORTC2 low : enable motor driver
	PORTC &= ~(1 << PORTC4);//PORTC4 low : enable motor driver
	
	//PORTC5 == 1 : left   PORTC5 == 0 : right,
	//
	
	InitializeTimer0();
	InitializeTimer1();
	InitializeTimer2();//PTB7을 OC2 핀으로 사용 OC1C 핀과 겹칩 주의
	InitializeTimer3();
	_delay_ms(5000);
	

	while(1) {
		PWMSet_TC3(3, 8.1);
		PWMSet_TC3(2, 8);
		PWMSet_TC3(1, 7);
		
		//MoveDown(200);
		MoveRight(20);
		PWMSet_TC3(1, 8);
		
		
		MoveUp(60);
		MoveLeft(20);
		
		_delay_ms(2000);
		
		PWMSet_TC3(1, 6.2);
		
		_delay_ms(2000);
		
		MoveUp(30);
		
		_delay_ms(2000);
		
		MoveRight(20);
		
		_delay_ms(2000);
		
		MoveUp(20);
	}
	
	return 0;
}