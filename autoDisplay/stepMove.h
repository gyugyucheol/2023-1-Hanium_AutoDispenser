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

void set_V_Enable() {
	PORTB &= ~(1 << PORTB2);
}
void set_V_Disable() {
	PORTB |= (1 << PORTB2);
}
void set_V_Up() {
	PORTB &= ~(1 << PORTB3);
}
void set_V_Down() {
	PORTB |= (1 << PORTB3);
}


void set_H_Enable() {
	PORTB &= ~(1 << PORTB5);
}
void set_H_Disable() {
	PORTB |= (1 << PORTB5);
}
void set_H_Right() {
	PORTB &= ~(1 << PORTB6);
}
void set_H_Left() {
	PORTB |= (1 << PORTB6);
}


void V_MoveStart(void) {
	//비교일치 발생시 핀 반전
	TCCR0 |= (1 << COM00);
	TCCR0 &= ~(1 << COM01);
}

void V_MoveStop(void) {
	//비교일치 발생시 핀 LOW
	TCCR0 &= ~(1 << COM00);
	TCCR0 |= (1 << COM01);
}

void H_MoveStart(void) {
	//비교일치 발생시 핀 반전
	TCCR2 |= (1 << COM20);
	TCCR2 &= ~(1 << COM21);
}

void H_MoveStop(void) {
	//비교일치 발생시 핀 LOW
	TCCR2 &= ~(1 << COM20);
	TCCR2 |= (1 << COM21);
}



void MoveXY_relative(int x_mm, int y_mm) {
	curX += x_mm;
	curY += y_mm;
	
	if(x_mm >= 0) set_H_Right();
	else {
		set_H_Left();
		x_mm = -x_mm;
	}
	
	if(y_mm >= 0) set_V_Up();
	else {
		set_V_Down();
		y_mm = -y_mm;
	}
	
	//여기서부터 요이~땅
	TCNT1 = 0;
	if(x_mm) {
		H_MoveStart();
		h_stopFlag = 0;
	}
	if(y_mm) {
		V_MoveStart();
		v_stopFlag = 0;
	}
	h_ms = x_mm*10;
	v_ms = y_mm*25;
	
	
}

void MoveXY_absolute(unsigned int dstX, unsigned int dstY) {
	int Xdistance = dstX-curX;
	int Ydistance = dstY-curY;
	
	curX = dstX;
	curY = dstY;
	
	if(Xdistance >= 0) set_H_Right();
	else {
		set_H_Left();
		Xdistance = -Xdistance;
	}
	
	if(Ydistance >= 0) set_V_Up();
	else {
		set_V_Down();
		Ydistance = -Ydistance;
	}
	
	//여기서부터 요이~땅
	TCNT1 = 0;
	if(Xdistance){
		H_MoveStart();
		h_stopFlag = 0;
	}
	if(Ydistance){
		V_MoveStart();
		v_stopFlag = 0;
	}
	h_ms = Xdistance*10;
	v_ms = Ydistance*25;
	
}

void pauseUntilStop(void) {
	while(h_stopFlag == 0 || v_stopFlag == 0) {};
}

		/*H_MoveStart()
		 *_delay_ms(1000): 100mm
		 *_delay_ms(2000): 200mm
		 *_delay_ms(3000): 300mm
		 * 가로축 최대 이동가능 전장 560mm 5600ms
		 */
		
		/*V_MoveStart()
		 *_delay_ms(1000): 40mm(예상)
		 *_delay_ms(2000): 80mm
		 * 세로축 최대 이동가능 전장 710mm 17750ms 
		 */

#endif /* STEPMOVE_H_ */