/*
 * servoMove1.h
 *
 * Created: 11/22/2022 9:00:07 PM
 *  Author: snowg
 */ 


#ifndef SERVOMOVE1_H_
#define SERVOMOVE1_H_

//1 : 800%(맞물려닫힘), 830%(작은 음료수캔 잡음), 900%(적당히벌림), 1000%(작은음료수캔 잡기전 벌림), 1300%(180도로 열림)
//2 : 300%(90도)~ 800%(0도,가로)~ 1300%(-90도)
//3 : 310%(안쪽 90도), 800%(0도), 1290%(-90도)
void PWMSet_TC3(int channel, unsigned int hectopercentage);
void MoveArm(unsigned int hand, unsigned int wrist, unsigned int elbow);

void basePoseArm(void);
void freePoseArm(void); 

void closeHand(void);
void catchHand_thin(void); 
void holdHand_thin(void); 
void openHand(void); 
void wideOpenHand(void); 


void CWturnWrist(void); 
void normalWrist(void); 
void CCWturnWrist(void); 


void innerTurnElbow(void);
void normalTurnElbow(void);
void outterTurnElbow(void);

#endif /* SERVOMOVE1_H_ */