#include <avr/io.h>
#include "Digital_Filter.h"
#include "Ring_Buffer.h"

struct Ring_Buffer_F inputs;
struct Ring_Buffer_F outputs;

//a[0] should be entered as the inverse of the coeff, for computation speed
//float b[5] = {0.0940, 0.3759, 0.5639, 0.3759, 0.0940};// cutoff frequency was 250
//float a[5] = {1.0000, 0.0000, 0.4860, 0.0000, 0.0177};
//float b[5] = {0.018563010626897, 0.074252042507589, 0.111378063761383, 0.074252042507589, 0.018563010626897}; // cutoff frequency was 150
//float a[5] = {1.000000000000000,  -1.570398851228171,  1.275613324983279, -0.484403368335085, 0.076197064610332};
//float b[5] = {4.165992044065786e-04, 0.001666396817626, 0.002499595226439, 0.001666396817626, 4.165992044065786e-04}; // cutoff frequency was 50
//float a[5] = {1.000000000000000,  -3.180638548874721, 3.861194348994217, -2.112155355110971, 0.438265142261981};	
float b[5] = {1.329372889879998e-05, 5.317491559519993e-05, 7.976237339279990e-05, 5.317491559519993e-05, 1.329372889879998e-05}; // cutoff frequency was 20
float a[5] = {1.000000000000000,  -3.671729089161935, 5.067998386734189, -3.115966925201744, 0.719910327291871};	
uint8_t i;

/* Initialization */
void digital_filter_init(){
	rb_initialize_F(&inputs);
	rb_initialize_F(&outputs);
	
	for(i = 0; i <= 15; i++){	
		rb_push_front_F(&inputs, 0);
		rb_push_front_F(&outputs, 0);
	}
	
	return;
}

float filterValue(float newInput){
	wrapPos(newInput);
	rb_pop_back_F(&inputs);
	rb_push_front_F(&inputs, newInput);
	float newOutput = 0.0;
	for(i=0; i <= 4; i++){	
		newOutput += b[i]*rb_get_F(&inputs,i);
		if (i>0){
			newOutput -= a[i]*rb_get_F(&outputs,i-1);
		}
	}
	newOutput *= a[0];
	rb_pop_back_F(&outputs);
	rb_push_front_F(&outputs, newOutput);
	newOutput = newOutput - rb_get_F(&outputs, 1);
	return newOutput;
}

void wrapPos(float newInput)
{
	if(rb_get_F(&outputs,0) - newInput > 180)
	{
		for(int i = 0; i <= rb_length_F(&outputs); i++)
		{
			rb_set_F(&inputs, i, rb_get_F(&inputs, i) - 360);
			rb_set_F(&outputs, i, rb_get_F(&outputs, i) - 360);
		}
	}
	else if(rb_get_F(&outputs,0) - newInput < -180)
	{
		for(int i = 0; i <= rb_length_F(&outputs); i++)
		{
			rb_set_F(&inputs, i, rb_get_F(&inputs, i) + 360);
			rb_set_F(&outputs, i, rb_get_F(&outputs, i) + 360);
		}
	}
}