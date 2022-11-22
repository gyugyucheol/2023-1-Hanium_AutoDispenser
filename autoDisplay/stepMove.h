/*
 * stepMove.h
 *
 * Created: 11/22/2022 9:00:21 PM
 *  Author: snowg
 */ 


#ifndef STEPMOVE_H_
#define STEPMOVE_H_

int curXpos = 0;
int curYpos = 0;

void Mydelay_ms(unsigned int mySecond)
{
	while(mySecond > 0)
	{
		_delay_ms(25);
		mySecond--;
	}
}

void verticalMove(void) {
	//비교일치 발생시 핀 반전
	TCCR0 |= (1 << COM00);
	TCCR0 &= ~(1 << COM01);
}

void verticalStop(void) {
	//비교일치 발생시 핀 LOW
	TCCR0 &= ~(1 << COM00);
	TCCR0 |= (1 << COM01);
}

void horizontalMove(void) {
	//비교일치 발생시 핀 반전
	TCCR2 |= (1 << COM20);
	TCCR2 &= ~(1 << COM21);
}

void horizontalStop(void) {
	//비교일치 발생시 핀 LOW
	TCCR2 &= ~(1 << COM20);
	TCCR2 |= (1 << COM21);
}

void MoveRight(int mm) {
	PORTC &= ~(1<<PORTC5);
	horizontalMove();
	Mydelay_ms(mm*2);
	horizontalStop();
	_delay_ms(200);
	curXpos += mm;
}

void MoveLeft(int mm) {
	PORTC |= (1<<PORTC5);
	horizontalMove();
	Mydelay_ms(mm*2);
	horizontalStop();
	_delay_ms(200);
	curXpos -= mm;
}

void MoveUp(int mm) {
	PORTC &= ~(1<<PORTC1);
	verticalMove();
	Mydelay_ms(mm);
	verticalStop();
	_delay_ms(200);
	curYpos += mm;
}

void MoveDown(double mm) {
	PORTC |= (1<<PORTC1);
	verticalMove();
	Mydelay_ms(mm);
	verticalStop();
	_delay_ms(200);
	curYpos -= mm;
}

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



#endif /* STEPMOVE_H_ */