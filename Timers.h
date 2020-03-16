/*
*
* Timers.h
*
* Created : 3/12/20 1:15pm
* Author : Anna Corman, Joslyne Lovelace, Megan Shapiro
*
*/

#ifndef TIMERS_H
#define TIMERS_H

/******* Functions ********/

//Initialize Timer 0
void timer0_init(int prescaler,int compVal);

//Initialize Timer 1
void timer1_init(int prescaler,int compVal);

//Initialize Timer 2
void timer2_init(int prescaler,int compVal);

#endif