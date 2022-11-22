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
16bit OCR, ICR은 16bit로 OCRnH, OCRnL로 1byte register로 나누어져있다.
하지만 OCRn, ICRn 으로 접근 가능
----------------------------------------*/


/* T/C configuration 법(16bit), (8bit)
 * 1. 파형 생성 모드 결정[WGMn3, WGMn2, WGMn1, WGMn0], [WGMn1, WGMn0]
 * 2. 파형 출력 모드 결정(파형 생성 모드에 따라 의미 다름)[COMnA1, COMnA0, COMnB1, COMnB0, COMnC1, COMnC0], [COMn1, COMn0]
 * 3. 분주비 설정[CSn2, CSn1, CSn0], [CSn2, CSn1, CSn0] //n=1,2,3은 같고 n=0은 다름
 * 4. 모드에 따라 파형 수치 OCR, ICR 값 설정(16bit T/C만 ICR이 있음)
 *
 * n=1 : 고속 PWM 모드, 비반전 모드, OCR(비교일치), ICR(TOP값) :
 * n=2 : CTC 모드, 비교일치시 반전 모드 OCR(TOP값) : 50% duty cycle
 * n=3 : 고속 PWM 모드, 비반전 모드, OCR(비교일치), ICR(TOP값)  
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
T/C0,2의 경우 PWM의 주기를 유연하게 조절하기 위해선 
파형생성모드를 CTC모드로 사용하는 선택지만 존재
CTC모드일 경우 PWM파형 출력을 중단하려면 
파형 출력 모드 [COMn1,COMn2]를 변경하여야 됨. 
---------------------*/

void InitializeTimer0(void) {//세로축 PWM: PTB4
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
	세로축 스텝모터 드라이버 입력 Microstep2, 입력 clock 2.5kHz 50%PWM 부터 안먹힌다. 
	-----------------------------------------------*/
}



void InitializeTimer2(void) {//가로축 PWM: PTB7
	
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
	세로축 스텝모터 드라이버 입력 Microstep2, 입력 clock 1.4kHz 50%PWM 부터 안먹힌다.
	-----------------------------------------------*/

}

void InitializeTimer1(void)
{/*
	DDRB |= (1 << PORTB5) | (1 << PORTB6);//clock
	
	
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	
	TCCR1A |= (1 << COM1A1);
	
	TCCR1B |= (1 << CS11);		
	
	ICR1 = ICR1VALUE-1;				// 2Mhz/40000 = 50Hz 
	OCR1A = ICR1;
	
	//OC1C는 OC2 핀과 겹치므로 사용 금지
*/}



void InitializeTimer3(void)
{
	DDRE |= (1 << PORTE3) | (1 << PORTE4) | (1 << PORTE5);
	
	//[WGMn3, WGMn2, WGMn1, WGMn0] = [1,1,1,0]
	//top값이 ICRn인 고속PWM모드
	TCCR3A |= (1 << WGM31);
	TCCR3B |= (1 << WGM32) | (1 << WGM33);
	
	//고속PWM에서
	//[COMnA1, COMnA0] = [1, 0]
	//비교 일치가 발생하면 OCnA 핀의 출력은 LOW값으로 바뀌고, BOTTOM에서 HIGH값으로 바뀐다(비반전 모드)
	TCCR3A &= ~(1 << COM3A0);
	TCCR3A |= (1 << COM3A1);
	
	TCCR3A &= ~(1 << COM3B0);
	TCCR3A |= (1 << COM3B1);
	
	TCCR3A &= ~(1 << COM3C0);
	TCCR3A |= (1 << COM3C1);
	
	TCCR3B |= (1 << CS11);	
	
	OCR3A = ICR3VALUE;
	OCR3B = ICR3VALUE;
	OCR3C = ICR3VALUE;
	
	//ICR 초기값 0
	ICR3 = ICR3VALUE-1;				// 2Mhz/40000 = 50Hz
	//이 때부터 timer 시작
}




#endif /* TC_CONFIG_H_ */