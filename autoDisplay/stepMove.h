/*
 * stepMove.h
 *
 * Created: 11/22/2022 9:00:21 PM
 *  Author: snowg
 */ 


#ifndef STEPMOVE_H_
#define STEPMOVE_H_

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



#endif /* STEPMOVE_H_ */