#include <avr/io.h>
#include "ADC.h"

void adc_init() {	
	//Set reference to built in channels
	ADMUX = (1<<REFS0);
	//Enable ADC w/ prescaler
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t adc_read(uint8_t ch) {
	//select channel to read
	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8)|ch;
	//start conversion
	ADCSRA |= (1<<ADSC);
	//wait for conversion to complete
	while(ADCSRA & (1<<ADSC));
	//return result
	return (ADC);
}
