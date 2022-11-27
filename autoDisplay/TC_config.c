/*
 * TC_config.c
 *
 * Created: 11/28/2022 7:02:07 AM
 *  Author: snowg
 */ 
#include <avr/io.h>
#include "TC_config.h"

void InitializeTimer0(void) {//??? PWM: PTB4
	//PORT config : out
	DDRB |= (1 << PORTB4);
	
	//1. CTC mode
	TCCR0 |= (1 << WGM01);//CTC mode
	
	//2. clear mode(temporary)
	TCCR0 &= ~(1 << COM00);
	TCCR0 |= (1 << COM01);
	
	//3. Prescaler : 32 -> 16MHz/32 = 500KHz
	TCCR0 |= (1 << CS00);
	TCCR0 |= (1 << CS01);
	
	//4. 500kHz/ ((124+1)*2) = 2000Hz
	OCR0 = 124;
	
	/*------OCR value-------------------------------
	OCR0 = 125 is best for now
	no below OCR0 = 100(2500Hz)
	??? ???? ???? ?? Microstep2, ?? clock 2.5kHz 50%PWM ?? ????. 
	-----------------------------------------------*/
}



void InitializeTimer2(void) {//??? PWM: PTB7
	
	//PORT config : out
	DDRB |= (1 << PORTB7);
	
	//1. CTC mode
	TCCR2 |= (1 << WGM21);
	
	//2. clear mode(temporary)
	TCCR2 &= ~(1 << COM20);
	TCCR2 |= (1 << COM21);
	
	//3. Prescaler : 64 -> 16MHz/64 = 250KHz
	TCCR2 |= (1 << CS20);
	TCCR2 |= (1 << CS21);
	
	//4. 250kHz/ ((124+1)*2) = 1000Hz
	OCR2 = 124;
	
	/*------OCR value-------------------------------
	OCR2 = 125 is best for now
	no below OCR2 = 100(1250Hz)
	??? ???? ???? ?? Microstep2, ?? clock 1.4kHz 50%PWM ?? ????.
	-----------------------------------------------*/

}

//OC1C? OC2 ?? ???? ?? ??
void InitializeTimer1(void)
{
	//(?? ?? ??)
	//DDRB |= (1 << PORTB5) | (1 << PORTB6);
	
	//1. top?? ICRn? ??PWM?? ([WGMn3, WGMn2, WGMn1, WGMn0] = [1,1,1,0])
	/*						TOP		update		TOVn Flag set
	 * 12: CTC mode			ICRn	immediate	MAX(0xFFFF)
	 * 14: fast PWM mode	ICRn	BOTTOM		TOP
	 */
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) 
		    | (1 << WGM13);
	
	// 2. ?? ?? ?? : ??(?? ?? ??)
	/*//default?? ??
	TCCR3A &= ~(1 << COM3A0);
	TCCR3A &= ~(1 << COM3A1);
	
	TCCR3A &= ~(1 << COM3B0);
	TCCR3A &= ~(1 << COM3B1);
	
	TCCR3A &= ~(1 << COM3C0);
	TCCR3A &= ~(1 << COM3C1);
	*/
	
	//3. Prescaler : 1 -> 16MHz/1 = 16MHz
	TCCR1B |= (1 << CS10);		
	
	ICR1 = 16000-1;				// 4. 16MHz/16000 = 1KHz 
	
	//overflow interrupt enable
	TIMSK |= (1<<TOIE1);
}


void InitializeTimer3(void)
{
	DDRE |= (1 << PORTE3) | (1 << PORTE4) | (1 << PORTE5);
	
	//1. top?? ICRn? ??PWM?? ([WGMn3, WGMn2, WGMn1, WGMn0] = [1,1,1,0])
	TCCR3A |= (1 << WGM31);
	TCCR3B |= (1 << WGM32) 
	        | (1 << WGM33);
			
	
	//2. ?? ?? ?? : ??PWM?? ??? ??([COMnA1, COMnA0] = [1, 0])
	//?? ??? ???? OCnA ?? ??? LOW??? ???, BOTTOM?? HIGH??? ???(??? ??)
	TCCR3A &= ~(1 << COM3A0);
	TCCR3A |= (1 << COM3A1);
	
	TCCR3A &= ~(1 << COM3B0);
	TCCR3A |= (1 << COM3B1);
	
	TCCR3A &= ~(1 << COM3C0);
	TCCR3A |= (1 << COM3C1);
	
	//3. Prescaler : 8 -> 16MHz/8 = 2MHz
	TCCR3B |= (1 << CS31);	
	
	OCR3A = 40000;
	OCR3B = 40000;
	OCR3C = 40000;
	
	//ICR ??? 0
	ICR3 = 40000-1;				// 4. 2MHz/40000 = 50Hz
	//? ??? timer ??
}
