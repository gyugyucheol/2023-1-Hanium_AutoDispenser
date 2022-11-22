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
		
		//1 : 6.2%(맞물려닫힘), 6.2%(작은 음료수캔 잡음), 7%(작은음료수캔 잡기전 벌림), 9.5%(180도로 열림), 11%(손등이 1자로 벌림)
		//2 : 3%(90도)~ 8%(0도,가로)~ 13%(-90도)
		//3 : 3.45%(안쪽 90도), 8.1%(0도), 12.9%(-90도)
		
		/*//동작 잘됨
		PORTC |= (1<<PORTC5);
		verticalMove();
		_delay_ms(1000);
		verticalStop();
		_delay_ms(1000);
		
		
		PORTC &= ~(1<<PORTC5);
		verticalMove();
		_delay_ms(1000);
		verticalStop();
		_delay_ms(1000);
		*/
		
		/*horizontalMove()
		 *_delay_ms(1000): 100mm
		 *_delay_ms(2000): 200mm
		 *_delay_ms(3000): 300mm
		 * 가로축 최대 이동가능 전장 560mm 5600ms
		 */
		
		/*verticalMove()
		 *_delay_ms(1000): 40mm(예상)
		 *_delay_ms(2000): 80mm
		 * 가로축 이동가능 전장 560mm 5600ms
		 */
		
		//가로축 이동가능 전장 560mm 5600mm
		//가로축 최대 이동가능 전장 710mm 17750ms 
		PORTC &= ~(1<<PORTC1);//
		verticalMove();
		_delay_ms(1500);
		
		verticalStop();
		PORTC |= (1<<PORTC0);
		_delay_ms(7000);
		PORTC &= ~(1<<PORTC0);	
		
		
		PORTC |= (1<<PORTC1);//
		verticalMove();
		_delay_ms(1500);
		
		verticalStop();
		PORTC |= (1<<PORTC0);
		_delay_ms(7000);
		PORTC &= ~(1<<PORTC0);	
	}
	
	return 0;
}


/*스텝모터 테스트 코드
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


		PWMSet_TC3(1,7.8);
		PWMSet_TC3(2,8);
		PWMSet_TC3(3,8.1);
		_delay_ms(5000);
		
		PWMSet_TC3(1,6.2);
		_delay_ms(5000);
*/