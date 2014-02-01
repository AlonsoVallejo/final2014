
#include "derivative.h"
#include "utils.h"
#include "cam.h"

uint32 bigcount=0;

void main(void) {
	//set oscilator clock to 50MHZ
	MCG_FEI_BLPE();
	//initialize uart
	init_uart();
	//initialize general purpose i/o
	init_gpio();
	//initialize analog to digital converter
	init_adc();
	//other initializations used in this experiment
	init_cam();

	while (1) {
		bigcount++;
	}//While(1)

}//Main