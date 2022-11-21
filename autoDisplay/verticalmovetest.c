#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define ROTATION_DELAY 	2000	

#define ICR1VALUE 2000



void InitializeTimer1(void)
{
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	
	TCCR1A |= (1 << COM1A1);
	
	TCCR1B |= (1 << CS11);		
	
	ICR1 = ICR1VALUE-1;				// 2Mhz/2000 = 1000Hz
	OCR1A = ICR1;
}



void OC2PinStart(void) {
	TCCR2 |= (1 << COM20);
	TCCR2 &= ~(1 << COM21);
}

void OC2PinStop(void) {
	TCCR2 &= ~(1 << COM20);
	TCCR2 |= (1 << COM21);
}

void InitializeTimer2(void)
{
	/*000 : no clock
	 *001 : 1
	 *010 : 8
	 *011 : 64
	 *100 : 256
	 *101 : 1024
	 *110 : external clock source
	 *111 : external clock source
	 */
	OC2PinStop();
	TCCR2 |= (1 << WGM21);//CTC mode
	
	//prescaler : 64 : 250KHz
	TCCR2 |= (1 << CS20); 
	TCCR2 |= (1 << CS21);
	
	//clock to Tb6560 : 250KHz / (OCR2*2)
	OCR2 = 60;// 250kHz/ (60*2) = 2083Hz
	//no below 50, 60 is best for now
	

}

int main(void)
{
	// ?? ?? ?? ???? ??
	DDRA |= (1 << PORTA0);
	
	//X? ???? EN, CW, clock
	DDRB |= (1 << PORTB5);//clock
	DDRC |= (1 << PORTC1) | (1 << PORTC2);//PORTC1 : CW, PORTC2 : EN;
	
	//Y? ???? EN, CW, clock
	DDRB |= (1 << PORTB7);
	DDRC |= (1 << PORTC3) | (1 << PORTC4);//PORTC3 : CW, PORTC4 : EN;

	DDRB |= (1 << PORTB4);

	PORTC &= ~(1 << PORTC2);//PORTC2 low : enable motor driver
	PORTC &= ~(1 << PORTC4);//PORTC4 low : enable motor driver
	
	InitializeTimer1();
	InitializeTimer2();
	_delay_ms(5000);
	
	while(1)
	{ 
		
		PORTC |= (1 << PORTC1);//PORTC1 high : CCW : leftmove
		OCR1A = ICR1VALUE/2;
		
		PORTC |= (1 << PORTC3);//PORTC3 high : CCW : down
		OC2PinStart();
		
		PORTA &= ~(1 << PORTA0);
		_delay_ms(1000);
		
		OC2PinStop();
		OCR1A = ICR1VALUE;
		_delay_ms(1000);
		
		PORTC &= ~(1 << PORTC1);//PORTC1 low : CW :rightmove
		OCR1A = ICR1VALUE/2;
		
		PORTC &= ~(1 << PORTC3);//PORTC3 low : CW : up
		OC2PinStart();
		PORTA |= (1 << PORTA0);
		_delay_ms(1000);
		
		OC2PinStop();
		OCR1A = ICR1VALUE;
		_delay_ms(1000);
	}

	return 0;
}