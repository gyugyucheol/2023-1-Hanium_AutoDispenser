/*
 * oldCode.h
 *
 * Created: 11/27/2022 11:44:02 PM
 *  Author: snowg
 */ 


#ifndef OLDCODE_H_
#define OLDCODE_H_


//linear rail move with _delay_ms
//200mm ?????? ???? ??? ?? ??! 2mm ??
//???? ?
void Mydelay_ms(unsigned int mySecond){
	while(mySecond > 0){
		_delay_ms(1);
		mySecond--;
	}
}

void MoveRight(int mm) {
	set_H_Right();
	H_MoveStart();
	Mydelay_ms(mm*10);
	H_MoveStop();
	_delay_ms(200);
	curX += mm;
}

void MoveLeft(int mm) {
	set_H_Left();
	H_MoveStart();
	Mydelay_ms(mm*10);
	H_MoveStop();
	_delay_ms(200);
	curX -= mm;
}

void MoveUp(int mm) {
	set_V_Up();
	V_MoveStart();
	Mydelay_ms(mm*25);
	V_MoveStop();
	_delay_ms(200);
	curY += mm;
}

void MoveDown(int mm) {
	set_V_Down();
	V_MoveStart();
	Mydelay_ms(mm*25);
	V_MoveStop();
	_delay_ms(200);
	curY -= mm;
}



#endif /* OLDCODE_H_ */