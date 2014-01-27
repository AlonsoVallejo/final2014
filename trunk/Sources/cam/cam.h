#ifndef CAM_H_
#define CAM_H_

/*
 * SI PTC0    9
 * SCLK PTC1 10
 * GND 		 19
 * VCC 	 	  2
 * AOUT		 18
 */ 
#define NUM_CLOCKS 256   

#define HALF_CLK_TIMER  0x8FF                		// half CLK period timer value  
#define QUARTER_CLK_TIMER  HALF_CLK_TIMER/2
                    		// half-clocks actually
#define DELTA 8                              		// additional delay //added
#define SI_TIMER HALF_CLK_TIMER*(NUM_CLOCKS+DELTA)  // min 256 


#define SI_PIN 0									// SI - pin 0 of PORTC
#define CLK_PIN 1									// CLK - pin 1 of PORTC
#define SC1A_VAL (ADC_SC1_ADCH(19)|ADC_SC1_AIEN_MASK) // ADC1 channel and interrupt enable  

void init_cam( );

#endif /* PIT_H_ */
