/*
*
* ADC.h
*
* Created : 3/15/20 1:15pm
* Author : Anna Corman, Joslyne Lovelace, Megan Shapiro
*
*/ 


#ifndef ADC_H
#define ADC_H

/******* Functions ********/

// initialize adc
void adc_init();

// read analog signal
uint16_t adc_read(uint8_t ch);

#endif