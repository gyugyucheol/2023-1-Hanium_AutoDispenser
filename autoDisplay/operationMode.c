/*
 * operationMode.c
 *
 * Created: 11/28/2022 6:38:32 AM
 *  Author: snowg
 */ 
#define F_CPU 16000000L
#include <util/delay.h>
#include <stdio.h>

#include "global_variable.h"
#include "operationMode.h"

#include <avr/io.h>
#include <stdio.h>
#include "UART0.h"
#include "UART1.h"

extern FILE OUTPUT;
extern FILE INPUT;

char uartBuffer[20] = "0";

void testMode() {
	int quit = 0;
	int wronginput=0;
	int testModeSelect;
	
	int coordinate_X, coordinate_Y;
	unsigned int hand,  wrist, elbow;
	
	while(1) {
		do {
			do {
				wronginput = 0;
				
				printf("Select mode : step_rel(1), step_abs(2), servo_con(3), quit(q)?");
				scanf("%s", uartBuffer);
				
				if(strcasecmp(uartBuffer, "1")==0)
				testModeSelect = 1;
				else if (strcasecmp(uartBuffer, "2")==0)
				testModeSelect = 2;
				else if (strcasecmp(uartBuffer, "3")==0)
				testModeSelect = 3;
				else if (strcasecmp(uartBuffer, "q")==0)
				quit = 1;
				else{
					printf("input '1', '2', '3' or 'q'\r\n");
					wronginput = 1;
				}
			} while(wronginput);
			
			if(quit) break;
			
			if(testModeSelect==1||testModeSelect==2) {
				printf("X coordinate : ");
				scanf("%u", &coordinate_X);
				printf("your input : %5d\r\n\r\n", coordinate_X);
				
				printf("Y coordinate : ");
				scanf("%u", &coordinate_Y);
				printf("your input : %5d\r\n\r\n", coordinate_Y);
				
				printf("[x,y] = [%5d, %5d]\r\n",coordinate_X, coordinate_Y);
			}
			else if(testModeSelect==3) {
				printf("hand  : ");
				scanf("%u", &hand);
				printf("wrist : ");
				scanf("%u", &wrist);
				printf("elbow : ");
				scanf("%u", &elbow);
			}
			
			do {
				wronginput = 0;
				
				printf("Do you want to proceed?[y/n]");
				scanf("%s", uartBuffer);
				
				if(strcasecmp(uartBuffer, "y")&&strcasecmp(uartBuffer, "n")) {
					printf("input 'y' or 'n' only\r\n");
					wronginput = 1;
				}
			} while(wronginput);
			
			printf("\r\n");
		} while(strcasecmp(uartBuffer, "y"));
		
		if(quit) break;
		else if(testModeSelect == 1) {
			MoveXY_relative(coordinate_X, coordinate_Y);
			pauseUntilStop();
			printf("current Position : [%4d, %4d]\r\n\r\n", curX, curY);
			_delay_ms(1000);
		}
		else if(testModeSelect == 2) {
			MoveXY_absolute(coordinate_X, coordinate_Y);
			pauseUntilStop();
			printf("current Position : [%4d, %4d]\r\n\r\n", curX, curY);
			_delay_ms(1000);
		}
		else if(testModeSelect == 3){
			MoveArm(hand,wrist,elbow);
		}
		
	}
}

void manualFulfillMode(void) {
	unsigned int row;
	unsigned int column;
	
	int wronginput=0;
	int quit = 0;
	
	setHereas00();
	
	while(1) {
		do{
			quit = 0;
			printf("n");
			printf("\r\n");
			printf("(forUser)@@quit for row == 4@@\r\n");
			printf("(forUser)waiting for [row,column] : ");
			scanf("%u,%u", &row, &column);
			
			while(row>=5||column>=9) {
				printf("r");
				printf("\r\n");
				printf("(forUser)worng input");
				printf("(forUser)quit for row == 4\r\n");
				printf("(forUser)waiting for [row,column] : ");
				scanf("%u,%u", &row, &column);
			}
			
			do {
				wronginput = 0;
				if(row==4) {
					printf("(forUser)quit - ");
					quit = 1;
					} else {
					printf("(forUser)[%u,%u] - ",row, column);
				}
				
				printf("Do you want to proceed?[y/n] : ");
				scanf("%s", uartBuffer);
				
				if(strcasecmp(uartBuffer, "y")&&strcasecmp(uartBuffer, "n")) {
					printf("input 'y' or 'n' only\r\n");
					wronginput = 1;
				}
			} while(wronginput);
			printf("\r\n");
		} while(strcasecmp(uartBuffer, "y"));
		if(quit == 1) break;
		
		//시작 준비------------------------------------------------
		printf("location : [%u,%u]\r\n", XlocArr[row][column],  YlocArr[row]);
		
		fulfill(row, column);
	}
}

void fulfill(int row, int column) {
	basePoseArm();
	_delay_ms(500);
	//---------------------------------------------------------
	
	//음료수 집음-----------------------------------------------
	MoveXY_absolute(140, 210);
	pauseUntilStop();
	_delay_ms(500);
	
	openHand();
	
	MoveXY_absolute(0, 210);
	pauseUntilStop();
	_delay_ms(200);
	
	catchHand_thin();
	_delay_ms(200);
	
	MoveXY_relative(0, 40);
	pauseUntilStop();
	_delay_ms(100);
	MoveXY_relative(100, 0);
	pauseUntilStop();
	_delay_ms(100);
	//-----------------------------------------------------------
	
	//음료수 집은 후 지정된 위치로 이동--------------------
	if(XlocArr[row][column] <= 250) {
		MoveXY_absolute(250, YlocArr[row]);
		pauseUntilStop();
		_delay_ms(200);
		
		CWturnWrist();
		_delay_ms(500);
		innerTurnElbow();
		_delay_ms(500);
		
		
		MoveXY_absolute(XlocArr[row][column], YlocArr[row]);
		pauseUntilStop();
		_delay_ms(200);
		} else {
		MoveXY_absolute(XlocArr[row][column], YlocArr[row]);
		pauseUntilStop();
		_delay_ms(200);
		
		CWturnWrist();
		_delay_ms(500);
		innerTurnElbow();
		_delay_ms(500);
	}
	//---------------------------------------------------------
	
	//음료수 놓음---------------------------------------------
	holdHand_thin();
	_delay_ms(100);
	MoveXY_relative(0, -50);
	pauseUntilStop();
	_delay_ms(200);
	
	openHand();
	_delay_ms(1500);
	
	MoveXY_relative(0, 50);
	pauseUntilStop();
	_delay_ms(200);
	//----------------------------------------------------------
	
	//팔 뺌-----------------------------------------------------
	if(XlocArr[row][column] <= 250) {
		MoveXY_absolute(250, YlocArr[row]);
		pauseUntilStop();
		_delay_ms(200);
	}
	
	closeHand();
	_delay_ms(200);
	
	normalTurnElbow();
	_delay_ms(500);
	normalWrist();
	_delay_ms(500);
	//----------------------------------------------------------
	
	//종료------------------------------------------------------
	//---------------------------------------------------------
}

void manualMode(void) {
	int wronginput=0;
	
	set_V_Disable();//PORTC0 low : disable vertical motor
	set_H_Disable();//PORTC4 low : disable horizontal motor
	freePoseArm();
	
	while(1) {
		printf("Moveup(1), Movedown(2), gotoMainMenu(q)?");
		scanf("%s", uartBuffer);
		
		if(strcasecmp(uartBuffer, "1")==0) {
			printf("ready?(any word) : ");
			scanf("%s",uartBuffer);
			set_V_Enable();
			set_V_Up();
			V_MoveStart();
			printf("stop(any word) : ");
			scanf("%s",uartBuffer);
			V_MoveStop();
			set_V_Disable();
		}
		else if (strcasecmp(uartBuffer, "2")==0){
			printf("ready?(any word) : ");
			scanf("%s",uartBuffer);
			set_V_Enable();
			set_V_Down();
			V_MoveStart();
			printf("stop(any word) : ");
			scanf("%s",uartBuffer);
			V_MoveStop();
			set_V_Disable();
		}
		else if (strcasecmp(uartBuffer, "q")==0)
		break;
		else
		printf("input '1', '2' or 'q'\r\n");
	}
	
	basePoseArm();
	
	set_V_Enable();//PORTC0 low : enable vertical motor
	set_H_Enable();//PORTC4 low : enable horizontal motor
}

void setHereas00(void) {
	curX = 0; curY = 0;
}


void autoFulfillMode(void) {
	unsigned int row;
	unsigned int column;
	
	OUTPUT.put = UART0_transmit;
	OUTPUT.get = NULL;
	OUTPUT.flags = _FDEV_SETUP_WRITE;

	INPUT.put = NULL;
	INPUT.get = UART0_receive;
	INPUT.flags = _FDEV_SETUP_READ;
	
	setHereas00();
	
	while(1) {
		if(UCSR1A & (1<<RXC1)) {
			
			OUTPUT.put = UART1_transmit;
			OUTPUT.get = NULL;
			OUTPUT.flags = _FDEV_SETUP_WRITE;

			INPUT.put = NULL;
			INPUT.get = UART1_receive;
			INPUT.flags = _FDEV_SETUP_READ;
			
			printf("\r\n get out from autoFulfuillMode\r\n");
			break;
		} 
		
		printf("n");
		scanf("%u,%u", &row, &column);
		
		while(row>=5||column>=9) {
			printf("r");
			scanf("%u,%u", &row, &column);
		}
		
		if(row==4) {
			OUTPUT.put = UART1_transmit;
			OUTPUT.get = NULL;
			OUTPUT.flags = _FDEV_SETUP_WRITE;

			INPUT.put = NULL;
			INPUT.get = UART1_receive;
			INPUT.flags = _FDEV_SETUP_READ;
			
			printf("\r\n get out from autoFulfuillMode\r\n");
			break;
		}
		
		fulfill(row, column);
	}
}