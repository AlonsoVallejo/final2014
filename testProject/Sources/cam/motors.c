/*
 * motors.c
 *
 *  Created on: 16.01.2014
 *      Author: Iulian
 */

#include "motors.h"
#include "pwm.h"
#include "utils.h"
#include "gpio.h"


static int crnt_speed = 0;
static short int sign = 0;
short int reference = 0;
short int forward = 1; //and 0 brake
short int DEFAULT_STEP = 1;
extern int brake;
extern int PWMR_ABS;
extern int PWML_ABS;
extern unsigned char turatie_crt;
extern unsigned char turatie_ref;
extern int pwm_crt;

int count_pit0 = 0;
const unsigned char get_spd(){
	return crnt_speed;
}

// valoarea setata in urma ultimei comenzi start_chspeed()
const unsigned char get_reference() {
	return reference ;
}

void disable_motors() {
	ENDL_LOW;
	ENDR_LOW;
}

void enable_motors() {
	ENDL_HIGH;
	ENDR_HIGH;
}

void dumb_sleep(uint32 count) {
	int i;
	for (i = 0; i < count; i++) {
	}
}

void set_direction(int forw) {
	if (forw && !forward) {
		disable_motors();
		//stop_chspeed();
		//reference = 0;
		dumb_sleep(US_TO_CLOCKS(10)); 
		GPIOA_PCOR = GPIO_PIN(5); 
		//GPIOB_PCOR = GPIO_PIN(23);
		enable_motors();
		forward = 1;
	}
	else if (!forw && forward) {
		disable_motors();
		//stop_chspeed();
		//reference = 0;
		dumb_sleep(US_TO_CLOCKS(10));
		GPIOA_PSOR = GPIO_PIN(5); 
		//GPIOB_PSOR = GPIO_PIN(23);
		enable_motors();
		forward = 0;
	}
}

// apelata periodic
void update_speed(){ 
	
	int err = turatie_ref - turatie_crt;
	if( err > 10 )
		err = 10 ;
	if(err < -10)
		err = -10;
	
	pwm_crt+=TURATIE_TO_PWM(err);
	if(pwm_crt > 200)
		pwm_crt = 200;
	if(pwm_crt<0)
		pwm_crt=0;
	
	
	SET_DUTY_LEFT(pwm_crt);
	SET_DUTY_RIGHT(pwm_crt);
	
	LED2_TOGGLE;
	
	//io_printf("%d->%d\n",turatie_crt,pwm_crt);
	
	turatie_crt = 0;
	PIT_TFLG3 = 1; 						// clear interrupt flag for pit1
	PIT_TCTRL3 = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK; //workaround to re-enable interrupts
}


void start_dir_aware_chspeed(unsigned int clocks,short int reference_) {
	if (forward) start_chspeed(clocks,reference_);
	else start_chspeed(clocks,255-reference_);	
}


// 
void start_chspeed(unsigned int clocks,short int reference_){
	short int difference;
	stop_chspeed();
	if(get_reference()>reference_){brake = 1;}
	else brake = 0;
	reference = (reference_<0) ? 0: reference_ ;
	reference = (reference_>MAX_PWM) ? MAX_PWM: reference_;
	difference =  reference - crnt_speed;
	
	if (difference<0) { sign=-1; difference = -difference; }
	else if (difference>0) sign=1;
	else return;
	
	//protectie impotriva accelerarilor bruste - REVISE 
//	if (clocks/difference < MIN_CLOCKS_PER_DIFFERENCE_UP && sign>0) { 
//		//printf("DW\n");
//		return; }
	//protectie impotriva franarilor bruste - REVISE
//	if (clocks/difference < MIN_CLOCKS_PER_DIFFERENCE_DOWN && sign<0) { //printf("DW\n"); 
//		return; }

	// la fiecare LDVAL nr de clock-uri valoarea motoarelor va fi adunata cu DEFAULT_STEP
	PIT_LDVAL3 = clocks*DEFAULT_STEP/difference;	
	PIT_TCTRL3 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //enable pit0
	PIT_MCR = 1;						// enable module clock	
}

void stop_chspeed()
{
	reference = crnt_speed;
	PIT_TCTRL3 = 0;
}

void init_chspeed() {
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; 	//enable clock distribution to module	
	SET_DUTY_LEFT(-40);
	SET_DUTY_RIGHT(-40);
	io_printf("~|~\n");
	// set pit interrupt service routine  
	//disable_irq(71);
	
	PIT_LDVAL3 = MS_TO_CLOCKS(200);	
	PIT_TCTRL3 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //enable pit0
	PIT_MCR = 1;						// enable module clock	
		
	enable_irq(71);
	
	crnt_speed = 0;
	reference = 0;
}
