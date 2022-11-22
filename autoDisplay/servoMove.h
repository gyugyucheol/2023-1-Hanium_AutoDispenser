/*
 * servoMove1.h
 *
 * Created: 11/22/2022 9:00:07 PM
 *  Author: snowg
 */ 


#ifndef SERVOMOVE1_H_
#define SERVOMOVE1_H_

//1 : 6.2%(맞물려닫힘), 6.2%(작은 음료수캔 잡음), 7%(작은음료수캔 잡기전 벌림), 9.5%(180도로 열림), 11%(손등이 1자로 벌림)
//2 : 3%(90도)~ 8%(0도,가로)~ 13%(-90도)
//3 : 3.45%(안쪽 90도), 8.1%(0도), 12.9%(-90도)
void PWMSet_TC3(int channel, float percentage){
	switch(channel) {
		case 1 : OCR3A = percentage*400; break;
		case 2 : OCR3B = percentage*400; break;
		case 3 : OCR3C = percentage*400; break;
	}
}

#endif /* SERVOMOVE1_H_ */