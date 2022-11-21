/*
 * TC_config.h
 *
 * Created: 11/21/2022 10:59:57 PM
 *  Author: snowg
 */ 


#ifndef TC_CONFIG_H_
#define TC_CONFIG_H_

#define ICR1VALUE 40000
#define ICR3VALUE 40000
/*------------TIP1-----------------------------
16bit OCR, ICR? 16bit? OCRnH, OCRnL? 1byte register? ??????.
??? OCRn, ICRn ?? ?? ??
----------------------------------------*/


/* T/C configuration ?(16bit), (8bit)
 * 1. ?? ?? ?? ??[WGMn3, WGMn2, WGMn1, WGMn0], [WGMn1, WGMn0]
 * 2. ?? ?? ?? ??(?? ?? ??? ?? ?? ??)[COMnA1, COMnA0, COMnB1, COMnB0, COMnC1, COMnC0], [COMn1, COMn0]
 * 3. ??? ??[CSn2, CSn1, CSn0], [CSn2, CSn1, CSn0] //n=1,2,3? ?? n=0? ??
 * 4. ??? ?? ?? ?? OCR, ICR ? ??(16bit T/C? ICR? ??)
 *
 * n=1 : ?? PWM ??, ??? ??, OCR(????), ICR(TOP?) :
 * n=2 : CTC ??, ????? ?? ?? OCR(TOP?) : 50% duty cycle
 * n=3 : ?? PWM ??, ??? ??, OCR(????), ICR(TOP?)  
 */


/*(CSn2, CSn1, CSn0):n=0
 *n=0
 *000 : no clock
 *001 : 1
 *010 : 8
 *011 : 32
 *100 : 64
 *101 : 128
 *110 : 256
 *111 : 1024
 */


/*(CSn2, CSn1, CSn0):n=1,2,3
 *000 : no clock
 *001 : 1
 *010 : 8
 *011 : 64
 *100 : 256
 *101 : 1024
 *110 : external clock source
 *111 : external clock source
 */


/*---------------------
T/C0,2? ?? PWM? ??? ???? ???? ??? 
??????? CTC??? ???? ???? ??
CTC??? ?? PWM?? ??? ????? 
?? ?? ?? [COMn1,COMn2]? ????? ?. 
---------------------*/

void OC0PinStart(void) {
	//???? ??? ? ??
	TCCR0 |= (1 << COM00);
	TCCR0 &= ~(1 << COM01);
}

void OC0PinStop(void) {
	//???? ??? ? LOW
	TCCR0 &= ~(1 << COM00);
	TCCR0 |= (1 << COM01);
}

void InitializeTimer0(void) {//??? PWM: PTB4
	//PORT config : out
	DDRB |= (1 << PORTB4);
	
	//1. CTC mode
	TCCR0 |= (1 << WGM01);//CTC mode
	
	//2. clear mode(temporary)
	OC0PinStop();
	
	//3. Prescaler : 32 -> 16MHz/32 = 500KHz
	TCCR0 |= (1 << CS00);
	TCCR0 |= (1 << CS01);
	
	//4. 500kHz/ (125*2) = 2000Hz
	OCR0 = 125;
	
	/*------OCR value-------------------------------
	OCR0 = 125 is best for now
	no below OCR0 = 100(2500Hz)
	??? ???? ???? ?? Microstep2, ?? clock 2.5kHz 50%PWM ?? ????. 
	-----------------------------------------------*/
}


void OC2PinStart(void) {
	//???? ??? ? ??
	TCCR2 |= (1 << COM20);
	TCCR2 &= ~(1 << COM21);
}

void OC2PinStop(void) {
	//???? ??? ? LOW
	TCCR2 &= ~(1 << COM20);
	TCCR2 |= (1 << COM21);
}
void InitializeTimer2(void) {//??? PWM: PTB7
	
	//PORT config : out
	DDRB |= (1 << PORTB7);
	
	//1. CTC mode
	TCCR2 |= (1 << WGM21);
	
	//2. clear mode(temporary)
	OC2PinStop();
	
	//3. Prescaler : 64 -> 16MHz/64 = 250KHz
	TCCR2 |= (1 << CS20);
	TCCR2 |= (1 << CS21);
	
	//4. 250kHz/ (125*2) = 1000Hz
	OCR2 = 125;
	
	/*------OCR value-------------------------------
	OCR2 = 125 is best for now
	no below OCR2 = 100(1250Hz)
	??? ???? ???? ?? Microstep2, ?? clock 1.4kHz 50%PWM ?? ????.
	-----------------------------------------------*/

}

void InitializeTimer1(void)
{/*
	//??? clock
	DDRB |= (1 << PORTB5) | (1 << PORTB6);//clock
	
	
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	
	TCCR1A |= (1 << COM1A1);
	
	TCCR1B |= (1 << CS11);		
	
	ICR1 = ICR1VALUE-1;				// 2Mhz/40000 = 50Hz 
	OCR1A = ICR1;
	
	//OC1C? OC2 ?? ???? ?? ??
*/}



void InitializeTimer3(void)
{
	DDRE |= (1 << PORTE3) | (1 << PORTE4) | (1 << PORTE5);
	
	//[WGMn3, WGMn2, WGMn1, WGMn0] = [1,1,1,0]
	//top?? ICRn? ??PWM??
	TCCR3A |= (1 << WGM31);
	TCCR3B |= (1 << WGM32) | (1 << WGM33);
	
	//??PWM??
	//[COMnA1, COMnA0] = [1, 0]
	//?? ??? ???? OCnA ?? ??? LOW??? ???, BOTTOM?? HIGH??? ???(??? ??)
	TCCR3A &= ~(1 << COM3A0);
	TCCR3A |= (1 << COM3A1);
	
	TCCR3A &= ~(1 << COM3B0);
	TCCR3A |= (1 << COM3B1);
	
	TCCR3A &= ~(1 << COM3B0);
	TCCR3A |= (1 << COM3B1);
	
	TCCR3B |= (1 << CS11);	
	
	OCR3A = ICR3VALUE/2;
	OCR3B = ICR3VALUE/2;
	OCR3C = ICR3VALUE;
	
	//ICR ??? 0
	ICR3 = ICR3VALUE-1;				// 2Mhz/40000 = 50Hz
	//? ??? timer ??
}

int getPWMvalue_TC3(float percentage){
	return percentage*400;
}





#endif /* TC_CONFIG_H_ */