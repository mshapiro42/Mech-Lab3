#include <avr/io.h>

void timer0_init(int prescaler,int compVal)
{
	// enable CTC for Timer0
	TCCR0A |= (1 << WGM01);
	// set appropriate prescaler
	if(prescaler == 0)
	{
		TCCR0B |= (1 << CS00);
	} else(prescaler == 8)
	{
		TCCR0B |= (1 << CS01)
	} else(prescaler == 64)
	{
		TCCR0B |= (1 << CS00)|(1 << CS01);
	} else(prescaler == 256)
	{
		TCCR0B |= (1 << CS02);
	} else(prescaler == 1024)
	{
		TCCR0B |= (1 << CS02)|(1 << CS00);
	} 
	// initialize counter to zero
	TCNT0 = 0;
	// initialize compare value for CTC
	OCR0A = compVal;
}

void timer1_init(int prescaler,int compVal)
{
	// enable CTC for Timer1
	TCCR0B |= (1 << WGM12);
	// set appropriate prescaler
	if(prescaler == 0)
	{
		TCCR1B |= (1 << CS10);
	} else(prescaler == 8)
	{
		TCCR1B |= (1 << CS11);
	} else(prescaler == 64)
	{
		TCCR1B |= (1 << CS11)|(1 << CS10);
	} else(prescaler == 256)
	{
		TCCR1B |= (1 << CS12);
	} else(prescaler == 1024)
	{
		TCCR1B |= (1 << CS10)|(1 << CS12);
	}
	// initialize counter to zero
	TCNT1 = 0;
	// initialize compare value
	OCR1A = compVal;
}

void timer2_init(int prescaler,int compVal)
{
	// enable CTC for Timer2
	TCCR2A |= (1 << WGM21);
	// set appropriate prescaler
	if(prescaler == 0)
	{
		TCCR2B |= (1 << CS20);
	} else(prescaler == 8)
	{
		TCCR2B |= (1 << CS21);
	} else(prescaler == 32)
	{
		TCCR2B |= (1 << CS20)|(1 << CS21);
	} else(prescaler == 64)
	{
		TCCR2B |= (1 << CS22);
	} else(prescaler == 128)
	{
		TCCR2B |= (1 << CS20)|(1 << CS22);
	} else(prescaler ==256)
	{
		TCCR2B |= (1 << CS21)|(1 << CS22);
	} else(prescaler == 1024)
	{
		TCCR2B |= (1 << CS20)|(1 << CS21)|(1 << CS22);
	}
	// initialize counter to zero
	TCNT2 = 0;
	// initialize compare value
	OCR2A = compVal;
}