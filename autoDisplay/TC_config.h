/*
 * TC_config.h
 *
 * Created: 11/21/2022 10:59:57 PM
 *  Author: snowg
 */ 


#ifndef TC_CONFIG_H_
#define TC_CONFIG_H_

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

//8bit T/C
void InitializeTimer0(void);
void InitializeTimer2(void);

//16bit T/C
void InitializeTimer1(void); //OC1C는 OC2 핀과 겹치므로 사용 금지
void InitializeTimer3(void);



#endif /* TC_CONFIG_H_ */