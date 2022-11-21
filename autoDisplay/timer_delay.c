#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define ROTATION_DELAY 	2000	// 1초 대기

#define ICR1VALUE 2000

//microstep=2, 한 클릭 주기가 1ms 일 때, 1000Hz의 PWM 주파수 필요
//400clock == 한바퀴 주기 == 400ms 가 된다.

void InitializeTimer1(void)
{
	// 모드 14, 고속 PWM 모드
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	
	// 비반전 모드
	// TOP : ICR1, 비교일치값 : OCR1A 레지스터
	TCCR1A |= (1 << COM1A1);
	
	
	TCCR1B |= (1 << CS11);		// 분주율 8, 2MHz, 다음 분주비는 64임
	
	ICR1 = ICR1VALUE-1;				// 2Mhz/2000 = 1000Hz
	OCR1A = 0;
}

int main(void)
{
	// 모터 제어 핀을 출력으로 설정
	DDRA |= (1 << PORTA0);
	DDRB |= (1 << PORTB5);
	DDRC |= (1 << PORTC1) | (1 << PORTC2);//PORTC1 : CW, PORTC2 : EN;

	PORTC &= ~(1 << PORTC2);//PORTC1 low : enable motor driver
	
	InitializeTimer1();			// 1번 타이머/카운터 설정
	_delay_ms(50000);
	
	
	while(1)
	{
		PORTC |= (1 << PORTC1);//PORTC1 low : clockwise
		OCR1A = ICR1VALUE/2;
		PORTA &= ~(1 << PORTA0);		
		_delay_ms(1000);
		
		OCR1A = ICR1VALUE;
		_delay_ms(1000);
		
		PORTC &= ~(1 << PORTC1);//PORTC1 high : counterclockwise
		OCR1A = ICR1VALUE/2;
		PORTA |= (1 << PORTA0);	
		_delay_ms(1000);
		
		OCR1A = ICR1VALUE;
		_delay_ms(1000);
	}

	return 0;
}