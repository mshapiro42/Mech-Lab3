/*
 * Mech-Lab2.c
 *
 * Created: 2/12/2020 1:14:53 PM
 * Author : Anna Corman, Joslyne Lovelace, Megan Shapiro
 */ 

#define F_CPU 16000000L //Clock speed of Arduino 16 MHz
#define BAUD 9600       //Desired Baud Rate of Serial Communication
#define MYUBRR F_CPU/16/BAUD-1 //Calculated value of UBRR to initialize USART
//#define TRANSMIT_READY UCSR0A & (1<<UDRE0) 
//#define TIMER0_FLAG (TIFR0 & (1 << OCF0A))
//#define TIMER1_FLAG (TIFR1 & (1 << OCF1A))

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>
#include "Serial.h"
#include "Ring_Buffer.h"
#include "Digital_Filter.h"
#include "Timers.h"
#include "ADC.h"

void fastPWM_init();
void setNewPWM(int vel_des);

/*
union floatChars {
	float asFloat;
	char asChars[4];
};*/

int main(void)
{
	//struct Ring_Buffer_C output_queue;
	USART_Init(MYUBRR);
    //rb_initialize_C(&output_queue);

	timer0_init(1024,155);
	timer1_init(0,15999);
	fastPWM_init();
	adc_init();
	digital_filter_init(0);
	
	//Set AI0 to Output and rest as Input
	DDRC |= 0b00000001;
	//Set pin 11,13, and 8 as output for PWM, Dir, and enable, respectively
	DDRB |= 0b00101001;
	
	//Set output to 1 to power sensor
	PORTC |= 0b00000001;
	//Set enable pin as high
	PORTB |= 0b00000001;
	
	//Sampling frequency for converting to velocity, 1/0.001
	float sampPer = 1000;
	float volt = 0;
	float angPos = 0;
	float angPosLast = 0;
	float angVel = 0;
	float filteredVel = 0;
	//union floatChars printVal;
	int vel_des[3] = {24, 0, -24};
	int timer0Count = 0; //change to volatile if issues, I'm thinking the increment will keep this from being an issue though
	enum states{STOP = 0, CW = 1, CCW = 2} stateCur = STOP, stateLast = CW;
	float convertCoeff[] = {-354.5305, 7.2116, -0.0543, 1.9698E-4, -3.5356E-7, 3.0609E-10, -1.0193E-13};
	float tempSum;
	float voltTemp = 0;
	int duty = 200;

    while (1) 
    {
		OCR2A = duty; // deadband at about 10
		//if TIMER0_flag
		if(TIFR0 & (1 << OCF0A))
		{
			timer0Count++;
			if(timer0Count == 50)
			{
				// Check for next action
				if(stateCur == 0 && stateLast == 1)
				{
					stateLast = stateCur;
					stateCur = CCW;
					setNewPWM(vel_des[2]); 
					duty = 200;
				} else if(stateCur == 0 && stateLast == 2)
				{
					stateLast = stateCur;
					stateCur = CW;
					setNewPWM(vel_des[0]);
					duty = 200;
				} else
				{
					stateLast = stateCur;
					stateCur = STOP;
					setNewPWM(vel_des[1]);
					duty = 0;
				}
				timer0Count = 0;
			}
			/*printVal.asFloat = 500; //edit so we don't drop readings during prints
			printVal.asFloat = angPos;
			for(int i = 0; i < 4; i ++){
				rb_push_back_C(&output_queue, printVal.asChars[i]);
			}*/
			print_float(filteredVel);
			//reset TIMER0_flag
			TIFR0 |= (1 << OCF0A);
		}
		//if TIMER1_flag
		if(TIFR1 & (1 << OCF1A))
		{
			//read voltage 
			volt = adc_read(1);	
			voltTemp = volt;
			//convert to position in radians
			tempSum = convertCoeff[0];
			// Apply 6th order best fit line found in Matlab
			for (int i = 1; i <= 6; i++){
				tempSum += convertCoeff[i]*voltTemp;
				voltTemp *= volt;
			}
			//wrap result
			angPos = tempSum;

			//convert to velocity
			angVel = (angPos - angPosLast) *0.00277778*sampPer; // rev/s
			
			//add angPos to queue
			angPosLast = angPos;
			
			//filter velocity
			filteredVel = filterValue(angVel);
			
			//reset TIMER1_flag
			TIFR1 |= (1 << OCF1A);
		} 
		/*if (rb_length_C(&output_queue) > 0 && TRANSMIT_READY){
			print_byte(rb_pop_front_C(&output_queue));
		}*/
    }
}

void fastPWM_init()
{
	// set Fast PWM mode on Timer 2 non-inverting (just add (1 << COM2A0) for inverting
	TCCR2A |= (1 << WGM20)|(1 << WGM21)|(1 << COM2A1);
	TCCR2B |= (1 << CS20)|(1 << CS21)|(1 << CS22);
}

void setNewPWM(int vel_des)
{
	if(vel_des > 0)
	{
		PORTB |= 0b00000001;
		TCCR2A |= (1 << WGM20)|(1 << WGM21)|(1 << COM2A1);
	}
	else if (vel_des > 0)
	{
		PORTB |= 0b00100001;
		TCCR2A |= (1 << WGM20)|(1 << WGM21)|(1 << COM2A1)|(1 << COM2A0);
	}
}