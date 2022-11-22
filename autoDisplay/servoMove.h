/*
 * servoMove1.h
 *
 * Created: 11/22/2022 9:00:07 PM
 *  Author: snowg
 */ 


#ifndef SERVOMOVE1_H_
#define SERVOMOVE1_H_


void PWMSet_TC3(int channel, float percentage){
	switch(channel) {
		case 1 : OCR3A = percentage*400; break;
		case 2 : OCR3B = percentage*400; break;
		case 3 : OCR3C = percentage*400; break;
	}
}

#endif /* SERVOMOVE1_H_ */