/*
 * stepMove.h
 *
 * Created: 11/22/2022 9:00:21 PM
 *  Author: snowg
 */ 


#ifndef STEPMOVE_H_
#define STEPMOVE_H_


void set_V_Enable();
void set_V_Disable();
void set_V_Up();
void set_V_Down();

void set_H_Enable();
void set_H_Disable();
void set_H_Right();
void set_H_Left();


void V_MoveStart(void);
void V_MoveStop(void);
void H_MoveStart(void);
void H_MoveStop(void);

void MoveXY_relative(int x_mm, int y_mm) ;

void MoveXY_absolute(unsigned int dstX, unsigned int dstY);

void pauseUntilStop(void);

		/*H_MoveStart()
		 *_delay_ms(1000): 100mm
		 *_delay_ms(2000): 200mm
		 *_delay_ms(3000): 300mm
		 * 가로축 최대 이동가능 전장 560mm 5600ms
		 */
		
		/*V_MoveStart()
		 *_delay_ms(1000): 40mm(예상)
		 *_delay_ms(2000): 80mm
		 * 세로축 최대 이동가능 전장 710mm 17750ms 
		 */

#endif /* STEPMOVE_H_ */