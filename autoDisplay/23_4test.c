#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	//microstep = 1/2
	
	
	//2000us, 2000us를 주면 이상하게 작동안한다.
	
	DDRA |= 0x01;
	DDRC |= 0x07;				// 모터 제어 핀을 출력으로 설정
	DDRB |= (1 << PORTB5);
	//PTC0 : clock
	//PTC1 : direction
	//PTC2 : enable

	PORTC &= ~(1<<PORTC2);//enable motor driver
	_delay_ms(5000);
	
	int halfperiod = 400;
	int microstep = 2;
	int oneround = 200; 
	int roundnum = 4; 
	
	int loopnum = microstep*oneround*roundnum;
	while(1)
	{
		
		//Motor rotate clockwise when is CW low level and counterclockwise when is CW high level
		//PTC1 : CW
		//400? ??? 1/2 microstep
		
		PORTC |= (1<<PORTC1);			// counterclockwise
		PORTA |= 0x01;			// LED off
		for(int i = 0; i <= loopnum; i++){		// 400 스텝 진행
			PORTB |= (1 << PORTB5);			// 상승 에지
			_delay_us(halfperiod);
			PORTB &= ~(1 << PORTB5);			// 하강 에지
			_delay_us(halfperiod);
		
			
		}
		_delay_ms(5000);

		PORTC &= ~(1<<PORTC1);			// clockwise
		PORTA &= ~0x01;			// LED on
		for(int i = 0; i < loopnum; i++){		// 400 스텝 진행
			PORTB |= (1 << PORTB5);			// 상승 에지
			_delay_us(halfperiod);
			PORTB &= ~(1 << PORTB5);			// 하강 에지
			_delay_us(halfperiod);
		}
		
		PORTC |= (1<<PORTC2);//disable motor driver;
		_delay_ms(5000);
		PORTC &= ~(1<<PORTC2);//enable motor driver
		
	}
	
	return 0;
}

