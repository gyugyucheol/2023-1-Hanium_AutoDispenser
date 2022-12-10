#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "TC_config.h"
#include "servoMove.h"
#include "stepMove.h"
#include "UART1.h"
#include "UART0.h"
#include "operationMode.h"


FILE OUTPUT = FDEV_SETUP_STREAM(UART1_transmit, NULL, _FDEV_SETUP_WRITE);
FILE INPUT = FDEV_SETUP_STREAM(NULL, UART1_receive, _FDEV_SETUP_READ);


//---------------grobal variable through file-------------------------
int curX = 0;
int curY = 0;

int h_ms = 0;
int v_ms = 0;

int h_stopFlag = 1;
int v_stopFlag = 1;

int XlocArr[4][9] = {
	{ 30, 110, 185, 260, 335, 420, 480, 550, 550},
	{ 25,  95, 160, 230, 295, 365, 430, 495, 550},
	{ 25,  95, 160, 230, 295, 365, 430, 495, 550},
	{ 25,  95, 160, 230, 295, 365, 430, 495, 550},
};
int YlocArr[4] = {50, 280, 510, 710};

//--------------------------------------------------------------------




ISR(INT0_vect) {//emergency switch
	freePoseArm();
	printf("\r\n\r\n!!!!INT0 set!!!!!\r\n\r\n");
	
	//set_V_Disable();//PORTC0 low : disable vertical motor
	//set_H_Disable();//PORTC4 low : disable horizontal motor
}

ISR(TIMER1_OVF_vect) {
	if(h_ms != -1) h_ms--;
	if(v_ms != -1) v_ms--;
	
	if(h_ms == 0) {
		H_MoveStop();
		h_stopFlag = 1;
	}
	if(v_ms == 0) {
		V_MoveStop();
		v_stopFlag = 1;
	}
}


int main(void)
{
	DDRB |= (1 << PORTB2) | (1 << PORTB3);//vertical, PORTB2 : EN, PORTB3 : CW;
	DDRB |= (1 << PORTB5) | (1 << PORTB6);//horizontal, PORTB5 : EN, PORTB6 : CW;

	PORTB &= ~(1 << PORTB2);//PORTC0 low : enable vertical motor
	PORTB &= ~(1 << PORTB5);//PORTC4 low : enable horizontal motor

	//T/C enable
	InitializeTimer0();//vertical step motor PWM channel
	InitializeTimer2();//horizontal step motor PWM channel
	InitializeTimer3();//robot arm servo motor PWM channel
	
	InitializeTimer1();//user timer
	TIMSK |= (1<<TOIE1); //T/C1 overflow interrupt enable
	
	//emergency button interrupt enable
	DDRD &= ~(1<<PORTD0);
	PORTD |= (1<<PORTD0);
	EIMSK |= (1<<INT0);
	EICRA |= (1<<ISC01);
	
	sei();
	
	UART1_init();
	stdout = &OUTPUT;
	stdin = &INPUT;
	
	UART0_init();
	
	
	basePoseArm();
	_delay_ms(5000);
	
	int wronginput;
	char uartBuffer[20] = "0";

	while(1) {
		do {
			wronginput = 0;
			
			printf("TestMode(1), manualFulfillMode(2), manualMode(3), setHereAs[0,0](4), autoFulfillMode(5)?");
			scanf("%s", uartBuffer);
			
			if(strcasecmp(uartBuffer, "1")==0) 
				testMode();
			else if (strcasecmp(uartBuffer, "2")==0) 
				manualFulfillMode();
			else if (strcasecmp(uartBuffer, "3")==0)
				manualMode();
			else if (strcasecmp(uartBuffer, "4")==0)
				setHereas00();
			else if (strcasecmp(uartBuffer, "5")==0)
				autoFulfillMode();
			else{
				printf("input '1', '2', '3' or '4'\r\n");
				wronginput = 1;
			}
		} while(wronginput);
	}
	
	return 0;
}