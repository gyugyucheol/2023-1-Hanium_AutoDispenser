/*
 * stepMove.c
 *
 * Created: 11/28/2022 6:22:48 AM
 *  Author: snowg
 */ 
#include <avr/io.h>
#include "global_variable.h"
#include "stepMove.h"


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
	//???? ??? ? ??
	TCCR0 |= (1 << COM00);
	TCCR0 &= ~(1 << COM01);
}

void V_MoveStop(void) {
	//???? ??? ? LOW
	TCCR0 &= ~(1 << COM00);
	TCCR0 |= (1 << COM01);
}

void H_MoveStart(void) {
	//???? ??? ? ??
	TCCR2 |= (1 << COM20);
	TCCR2 &= ~(1 << COM21);
}

void H_MoveStop(void) {
	//???? ??? ? LOW
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
	
	//????? ??~?
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
	
	//????? ??~?
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