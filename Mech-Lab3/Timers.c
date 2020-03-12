#include <avr/io.h>

void timer0_init(int prescaler,int compVal)
{
	// enable CTC for Timer0
	TCCR0A |= (1 << WGM01);
	if(prescaler == 1024)
	{
	// enable prescaler of 1024 for Timer0
	TCCR0B |= (1 << CS02)|(1 << CS00);
	}
	// initialize counter to zero
	TCNT0 = 0;
	// initialize compare value for CTC
	OCR0A = compVal;
}

void timer1_init(int prescaler,int compVal)
{
	TCCR1A |= 0;
	if(prescaler == 0)
	{
	// Enable CTC for Timer1 with no prescaler
	TCCR1B |= (1 << WGM12)|(1 << CS10);
	}
	// initialize counter to zero
	TCNT1 = 0;
	// initialize compare value
	OCR1B = 15999;
}