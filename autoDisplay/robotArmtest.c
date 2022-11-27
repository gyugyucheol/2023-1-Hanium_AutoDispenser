#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "TC_config.h"
#include "servoMove.h"
#include "stepMove.h"
#include "UART1.h"


FILE OUTPUT = FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT = FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);

int oprationMode;
int MoveMode_TM;
int coordinate_X, coordinate_Y;
char uartBuffer[20] = "0";
int wronginput=0;
unsigned int hand,  wrist, elbow;

int XlocArr[4][9] = {
	{ 50, 120, 200, 270, 350, 425, 500, 500, 500},
	{ 40, 105, 175, 240, 305, 365, 440, 510, 510,},
	{ 40, 105, 175, 240, 305, 365, 440, 510, 510,},
	{ 40, 105, 175, 240, 305, 365, 440, 510, 510,},
};
int YlocArr[4] = {50, 280, 510, 710};

void testmode() {
	int quit = 0;
	
	while(1) {
		do {
			do {
				wronginput = 0;
				
				printf("Select mode : step_rel(1), step_abs(2), servo_con(3), quit(q)?");
				scanf("%s", uartBuffer);
				
				if(strcasecmp(uartBuffer, "1")==0) 
					MoveMode_TM = 1;
				else if (strcasecmp(uartBuffer, "2")==0) 
					MoveMode_TM = 2;
				else if (strcasecmp(uartBuffer, "3")==0) 
					MoveMode_TM = 3;
				else if (strcasecmp(uartBuffer, "q")==0) 
					quit = 1;
				else{
					printf("input '1', '2', '3' or 'q'\r\n");
					wronginput = 1;
				}
			} while(wronginput);
			
			if(quit) break;
			
			if(MoveMode_TM==1||MoveMode_TM==2) {
				printf("X coordinate : ");
				scanf("%u", &coordinate_X);
				printf("your input : %5d\r\n\r\n", coordinate_X);
				
				printf("Y coordinate : ");
				scanf("%u", &coordinate_Y);
				printf("your input : %5d\r\n\r\n", coordinate_Y);
				
				printf("[x,y] = [%5d, %5d]\r\n",coordinate_X, coordinate_Y);
			}
			else if(MoveMode_TM==3) {
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
		else if(MoveMode_TM == 1) {
			MoveXY_relative(coordinate_X, coordinate_Y);
			pauseUntilStop();
			printf("current Position : [%4d, %4d]\r\n\r\n", curX, curY);
			_delay_ms(1000);
		}
		else if(MoveMode_TM == 2) {
			MoveXY_absolute(coordinate_X, coordinate_Y);
			pauseUntilStop();
			printf("current Position : [%4d, %4d]\r\n\r\n", curX, curY);
			_delay_ms(1000);
		}
		else if(MoveMode_TM == 3){
			MoveArm(hand,wrist,elbow);
		}
		
	}
}

void automode(void) {
	unsigned int row;
	unsigned int column;
	
	int quit = 0;
	
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
				printf("(forUser)@@quit for row == 4@@\r\n");
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
		basePoseArm();
		_delay_ms(1000);
		//---------------------------------------------------------
		
		//음료수 잡기-----------------------------------------------
		MoveXY_absolute(140, 210);
		pauseUntilStop();
		_delay_ms(500);
		
		openHand();
		
		MoveXY_absolute(0, 210);
		pauseUntilStop();
		_delay_ms(500);
		
		catchHand_thin();
		_delay_ms(500);
		
		MoveXY_relative(0, 40);
		pauseUntilStop();
		_delay_ms(300);
		MoveXY_relative(100, 0);
		pauseUntilStop();
		_delay_ms(300);
		//----------------------------------------------------------
		
		//지정된 좌표로 이동 및 안쪽으로 팔 집어넣음--------------------
		if(XlocArr[row][column] <= 250) {
			MoveXY_absolute(250, YlocArr[row]);
			pauseUntilStop();
			_delay_ms(500);
			
			CWturnWrist();
			_delay_ms(1000);
			innerTurnElbow();
			_delay_ms(1000);
			holdHand_thin();
			_delay_ms(1000);
			
			MoveXY_absolute(XlocArr[row][column], YlocArr[row]);
			pauseUntilStop();
			_delay_ms(500);
		} else {
			MoveXY_absolute(XlocArr[row][column], YlocArr[row]);
			pauseUntilStop();
			_delay_ms(500);
			
			CWturnWrist();
			_delay_ms(1000);
			innerTurnElbow();
			_delay_ms(1000);
			holdHand_thin();
			_delay_ms(1000);
		}
		//---------------------------------------------------------
		
		//살짝 내려놓음---------------------------------------------
		MoveXY_relative(0, -50);
		pauseUntilStop();
		_delay_ms(1000);
		
		wideOpenHand();
		_delay_ms(2000);
		
		MoveXY_relative(0, 50);
		pauseUntilStop();
		_delay_ms(1000);
		//----------------------------------------------------------
		
		//팔 뻄-----------------------------------------------------
		if(XlocArr[row][column] <= 250) {
			MoveXY_absolute(250, YlocArr[row]);
			pauseUntilStop();
			_delay_ms(1000);
		}
		
		closeHand_thin();
		_delay_ms(1000);
		
		normalTurnElbow();
		normalWrist();
		_delay_ms(1000);
		//----------------------------------------------------------
		
		//복귀------------------------------------------------------
		MoveXY_absolute(250, 150);
		pauseUntilStop();
		MoveXY_absolute(0, 0);
		pauseUntilStop();
		//---------------------------------------------------------
	}
}

void manualmode(void) {
	PORTC |= (1 << PORTC0);//PORTC0 low : disable vertical motor
	PORTC |= (1 << PORTC4);//PORTC4 low : disable horizontal motor
	freePoseArm();
	
	while(1) {
		printf("Moveup(1), Movedown(2), gotoMainMenu(q)?");
		scanf("%s", uartBuffer);
		
		if(strcasecmp(uartBuffer, "1")==0) {
			printf("ready?(any word) : ");
			scanf("%s",uartBuffer);
			PORTC &= ~(1 << PORTC0);
			PORTC &= ~(1<<PORTC1);
			verticalMove();
			printf("stop(any word) : ");
			scanf("%s",uartBuffer);
			verticalStop();
			PORTC |= (1 << PORTC0);
		}
		else if (strcasecmp(uartBuffer, "2")==0){
			printf("ready?(any word) : ");
			scanf("%s",uartBuffer);
			PORTC &= ~(1 << PORTC0);
			PORTC |= (1<<PORTC1);
			verticalMove();
			printf("stop(any word) : ");
			scanf("%s",uartBuffer);
			verticalStop();
			PORTC |= (1 << PORTC0);
		}
		else if (strcasecmp(uartBuffer, "q")==0)
			break;
		else
			printf("input '1', '2' or 'q'\r\n");
	}
	
	basePoseArm();
	
	PORTC &= ~(1 << PORTC0);//PORTC0 low : enable vertical motor
	PORTC &= ~(1 << PORTC4);//PORTC4 low : enable horizontal motor
}

void setHereas00(void) {
	curX = 0; curY = 0;
}

ISR(INT0_vect) {//emergency switch
	freePoseArm();
	
	PORTC |= (1 << PORTC0);//PORTC0 low : disable vertical motor
	PORTC |= (1 << PORTC4);//PORTC4 low : disable horizontal motor
}

ISR(TIMER1_OVF_vect) {
	if(h_ms != -1) h_ms--;
	if(v_ms != -1) v_ms--;
	
	if(h_ms == 0) {
		horizontalStop();
		h_stopFlag = 1;
	}
	if(v_ms == 0) {
		verticalStop();
		v_stopFlag = 1;
	}
}


int main(void)
{
	DDRC |= (1 << PORTC0) | (1 << PORTC1);//vertical, PORTC0 : EN, PORTC1 : CW;
	DDRC |= (1 << PORTC4) | (1 << PORTC5);//horizontal, PORTC4 : EN, PORTC5 : CW;

	PORTC &= ~(1 << PORTC0);//PORTC0 low : enable vertical motor
	PORTC &= ~(1 << PORTC4);//PORTC4 low : enable horizontal motor

	InitializeTimer0();//vertical step motor PWM channel
	InitializeTimer2();//horizontal step motor PWM channel
	InitializeTimer3();//robot arm servo motor PWM channel
	
	InitializeTimer1();//user timer
	TIMSK |= (1<<TOIE1); //T/C1 overflow interrupt enable
	
	DDRD &= ~(1<<PORTD0);
	PORTD |= (1<<PORTD0);
	EIMSK |= (1<<INT0);
	EICRA |= (1<<ISC01);
	
	sei();
	
	UART1_init();
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	
	basePoseArm();
	_delay_ms(5000);

	while(1) {
		do {
			wronginput = 0;
			
			printf("Testmode(1), automode(2), manualmode(3), setHereAs[0,0](4)?");
			scanf("%s", uartBuffer);
			
			if(strcasecmp(uartBuffer, "1")==0) 
				testmode();
			else if (strcasecmp(uartBuffer, "2")==0) 
				automode();
			else if (strcasecmp(uartBuffer, "3")==0)
				manualmode();
			else if (strcasecmp(uartBuffer, "4")==0)
				setHereas00();
			else{
				printf("input '1', '2', '3' or '4'\r\n");
				wronginput = 1;
			}
		} while(wronginput);
	}
	
	return 0;
}