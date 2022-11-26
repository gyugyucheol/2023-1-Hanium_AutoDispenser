#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "TC_config.h"
#include "servoMove.h"
#include "stepMove.h"

int state = 0;

ISR(TIMER1_OVF_vect) {
	state = !state;
	
	if(state) PORTC |= (1 << PORTC7);
	else PORTC &= ~(1 << PORTC7);
	
	if(h_ms != -1) h_ms--;
	if(v_ms != -1) v_ms--;
	
	if(h_ms == 0) {
		horizontalStop();
		h_stopFlag = 1;
	}
	if(v_ms == 0) {
		verticalStop();
		v_stopFlag = 1;
	}
}

ISR(TIMER1__vect) {
	state = !state;
	
	if(state) PORTC |= (1 << PORTC7);
	else PORTC &= ~(1 << PORTC7);
}


int main(void)
{
	DDRA |= (1 << PORTA0);
	
	DDRC |= (1 << PORTC0) | (1 << PORTC1);//vertical, PORTC0 : EN, PORTC1 : CW;
	DDRC |= (1 << PORTC4) | (1 << PORTC5);//horizontal, PORTC4 : EN, PORTC5 : CW;

	PORTC &= ~(1 << PORTC0);//PORTC2 low : enable motor driver
	PORTC &= ~(1 << PORTC4);//PORTC4 low : enable motor driver
	
	DDRC |= (1 << PORTC7);
	
	//PORTC5 == 1 : left   PORTC5 == 0 : right,
	

	InitializeTimer0();
	InitializeTimer2();//PTB7을 OC2 핀으로 사용 OC1C 핀과 겹칩 주의
	InitializeTimer3();
	
	InitializeTimer1();
	TIMSK |= (1<<TOIE1); //T/C1 overflow interrupt enable
	
	sei();
	PORTA &= ~(1 << PORTA0);
	_delay_ms(5000);
	
	
	PWMSet_TC3(3, 8.1);
	PWMSet_TC3(2, 8);
	PWMSet_TC3(1, 7);

	MoveDown(300);
	_delay_ms(10000);
	
	
	while(1) {
		
		MoveXY_relative(100,100);
		pauseUntilStop();
		_delay_ms(5000);
		
		
		MoveXY_relative(-100,-100);
		pauseUntilStop();
		_delay_ms(5000);
		
		MoveXY_relative(200,200);
		pauseUntilStop();
		_delay_ms(5000);
		
		MoveXY_relative(-200,-200);
		pauseUntilStop();
		_delay_ms(5000);
		
		
		MoveXY_absolute(200,200);
		pauseUntilStop();
		_delay_ms(5000);
		
		MoveXY_absolute(300,300);
		pauseUntilStop();
		_delay_ms(5000);
		
		MoveXY_absolute(0,0);
		pauseUntilStop();
		_delay_ms(5000);
	}
	
	return 0;
}
/*




MoveRight(200);

_delay_ms(10000);

MoveUp(200);

_delay_ms(2000);

MoveLeft(200);

_delay_ms(2000);

MoveDown(200);

_delay_ms(2000);
*/