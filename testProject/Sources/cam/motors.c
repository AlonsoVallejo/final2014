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
char velocity_state = OPEN_REACTION;
extern int brake;
extern int PWMR_ABS;
extern int PWML_ABS;
extern unsigned char turatie_crt;
extern unsigned char turatie_ref;
extern int pwm_crt;
extern unsigned char old_turatie_crt;
extern int config;

//2014
#define NM 2048
#define K1 	(3*NM)
#define K2  (1*NM)
#define K3  (1*NM/10)


const int b0 = K1 + K2/5+K3*5;
const int b1 =(-K1)-2*K3*5;
const int b2 =(K3)*5; 

int comm = 0;
int et1 = 0;
int et2 = 0;

extern const unsigned char scenarios[3][3];
int start_timer = 0 ; 
int count_pit0 = 0;
const unsigned char get_spd(){
	return crnt_speed;
}

// valoarea setata in urma ultimei comenzi start_chspeed()
const unsigned char get_reference() {
	return reference ;
}

void inline disable_motors() {
	ENDL_LOW;
	ENDR_LOW;
}

void inline enable_motors() {
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

void inline velocity_pid(int error)
{
	//pwm_crt+= (b0*error+ b1*et1+ b2*et2)/NM;
	//et2 = et1;
	//et1 = error;
	
	pwm_crt+=error;//the most empiric regulator
}

// apelata periodic
void update_speed()
{ 
	int err = turatie_ref - turatie_crt;
	
	if(turatie_ref !=0 )
	{
		switch(velocity_state)
		{
		case OPEN_REACTION:
			if(abs(err) < 4)
				velocity_state = ACCELERATE;
			else
				if(pwm_crt < 200)
					pwm_crt+=TURATIE_TO_PWM(5);
			SET_DUTY_LEFT(pwm_crt);
			SET_DUTY_RIGHT(pwm_crt);
			break;
		case ACCELERATE:
			velocity_pid(err);
			//io_printf("T:%d R:%d P:%d\n",turatie_crt,turatie_ref,pwm_crt);
			if(pwm_crt > 200)
				pwm_crt = 200;
			if(pwm_crt<0)
				pwm_crt=0;
			SET_DUTY_LEFT(pwm_crt);
			SET_DUTY_RIGHT(pwm_crt);
			break;
		}
	}
	else
	{
		start_timer++; 
		if (start_timer >= 100 )
		{
			turatie_ref = scenarios[config][ACCH_INDEX];
			start_timer = 0 ; 
			enable_motors();
			velocity_state = OPEN_REACTION ; 
		}
	}
	
	old_turatie_crt =  turatie_crt;
	LED2_TOGGLE;
	//io_printf("s:%d => %d->%d\n",velocity_state,turatie_crt,pwm_crt);
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
	
	PIT_LDVAL3 = MS_TO_CLOCKS(50);	
	PIT_TCTRL3 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //enable pit0
	PIT_MCR = 1;						// enable module clock	
		
	enable_irq(71);
	
	crnt_speed = 0;
	reference = 0;
}
