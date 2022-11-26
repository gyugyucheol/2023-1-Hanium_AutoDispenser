/*
 * stepMove.h
 *
 * Created: 11/22/2022 9:00:21 PM
 *  Author: snowg
 */ 


#ifndef STEPMOVE_H_
#define STEPMOVE_H_

int curX = 0;
int curY = 0;

int h_ms = 0;
int v_ms = 0;

int h_stopFlag = 1;
int v_stopFlag = 1;

//200mm 움직여봤는데 생가보다 오차가 크지 않다! 2mm 내외
//사용할만 함
void Mydelay_ms(unsigned int mySecond){
	while(mySecond > 0){
		_delay_ms(1);
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
	Mydelay_ms(mm*10);
	horizontalStop();
	_delay_ms(200);
	curX += mm;
}

void MoveLeft(int mm) {
	PORTC |= (1<<PORTC5);
	horizontalMove();
	Mydelay_ms(mm*10);
	horizontalStop();
	_delay_ms(200);
	curX -= mm;
}

void MoveUp(int mm) {
	PORTC &= ~(1<<PORTC1);
	verticalMove();
	Mydelay_ms(mm*25);
	verticalStop();
	_delay_ms(200);
	curY += mm;
}

void MoveDown(int mm) {
	PORTC |= (1<<PORTC1);
	verticalMove();
	Mydelay_ms(mm*25);
	verticalStop();
	_delay_ms(200);
	curY -= mm;
}

void MoveXY_relative(int x_mm, int y_mm) {
	if(x_mm >= 0) PORTC &= ~(1<<PORTC5);
	else PORTC |= (1<<PORTC5);
	
	if(y_mm >= 0) PORTC &= ~(1<<PORTC1);
	else PORTC |= (1<<PORTC1);
	
	h_stopFlag = 0;
	v_stopFlag = 0;
	
	TCNT1 = 0;
	horizontalMove();
	verticalMove();
	h_ms = x_mm*10;
	v_ms = y_mm*25;
	curX += x_mm;
	curY += y_mm;
}

void MoveXY_absolute(unsigned int dstX, unsigned int dstY) {
	int Xdistance = dstX-curX;
	int Ydistance = dstY-curY;
	
	if(Xdistance >= 0) PORTC &= ~(1<<PORTC5);
	else {
		PORTC |= (1<<PORTC5);
		Xdistance = -Xdistance;
	}
	
	if(Ydistance >= 0) PORTC &= ~(1<<PORTC1);
	else {
		PORTC |= (1<<PORTC1);
		Ydistance = -Ydistance;
	}
	
	h_stopFlag = 0;
	v_stopFlag = 0;
	
	TCNT1 = 0;
	horizontalMove();
	verticalMove();
	h_ms = Xdistance*10;
	v_ms = Ydistance*25;
	curX = dstX;
	curY = dstY;
}

void pauseUntilStop(void) {
	while(h_stopFlag == 1 || v_stopFlag == 1) {};
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
		
		//가로축 이동가능 전장 560mm 5600ms
		//가로축 최대 이동가능 전장 710mm 17750ms 



#endif /* STEPMOVE_H_ */