/*
 * servoMove.c
 *
 * Created: 11/28/2022 6:22:35 AM
 *  Author: snowg
 */ 

#include <avr/io.h>
#include "servoMove.h"

void PWMSet_TC3(int channel, unsigned int hectopercentage){
	switch(channel) {
		case 1 : OCR3A = hectopercentage*4; break;
		case 2 : OCR3B = hectopercentage*4; break;
		case 3 : OCR3C = hectopercentage*4; break;
	}
}

void MoveArm(unsigned int hand, unsigned int wrist, unsigned int elbow) {
	if(hand!=0)
	OCR3A = hand*4;
	if(wrist!=0)
	OCR3B = wrist*4;
	if(elbow!=0)
	OCR3C = elbow*4;
}

void basePoseArm(void) {
	OCR3A = 800*4;
	OCR3B = 800*4;
	OCR3C = 900*4;
}

void freePoseArm(void) {
	OCR3A = 3000*4;
	OCR3B = 3000*4;
	OCR3C = 3000*4;
}

void closeHand(void){
	OCR3A = 800*4;
}


void catchHand_thin(void) {
	OCR3A = 830*4;
}

void holdHand_thin(void) {
	OCR3A = 850*4;
}

void openHand(void) {
	OCR3A = 930*4;
}

void wideOpenHand(void) {
	OCR3A = 1200*4;
}


void CWturnWrist(void) {
	OCR3B = 300*4;
}

void normalWrist(void) {
	OCR3B = 800*4;
}

void CCWturnWrist(void) {
	OCR3B = 1300*4;
}


void innerTurnElbow(void) {
	OCR3C = 310*4;
}

void normalTurnElbow(void) {
	OCR3C = 900*4;
}

void outterTurnElbow(void) {
	OCR3C = 1290*4;
}