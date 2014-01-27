/*
 * motors.c
 *
 *  Created on: 16.01.2014
 *      Author: Iulian
 */

#include "motors.h"
#include "pwm.h"
#include "common.h"
#include "utils.h"


static int crnt_speed = 0;
static short int sign = 0;
short int reference = 0;
short int forward = 1; //and 0 brake
short int DEFAULT_STEP = 1;
extern int brake;
extern int PWMR_ABS;
extern int PWML_ABS;

int count_pit0 = 0;
const unsigned char get_spd(){
	return crnt_speed;
}

// valoarea setata in urma ultimei comenzi start_chspeed()
const unsigned char get_reference() {
	return reference ;
}

void disable_motors() {
	GPIOA_PCOR = GPIO_PIN(4);
	GPIOB_PCOR = GPIO_PIN(23);	
}

void enable_motors() {
	GPIOA_PSOR = GPIO_PIN(4);
	GPIOB_PSOR = GPIO_PIN(23);	
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
	crnt_speed+=DEFAULT_STEP*sign;
	if (sign<0 && crnt_speed<reference) { crnt_speed = reference; stop_chspeed(); } 
	if (sign>0 && crnt_speed>reference) { crnt_speed = reference; stop_chspeed(); }
	SET_DUTY_LEFT(crnt_speed);
	SET_DUTY_RIGHT(crnt_speed);
	
//	SET_DUTY_LEFT(PWML_ABS);
//	SET_DUTY_RIGHT(PWMR_ABS);
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
	
	// set pit interrupt service routine  
	disable_irq(71);
	__VECTOR_RAM[87] = (uint32)&update_speed; 
	enable_irq(71);
	
	crnt_speed = 0;
	reference = 0;
}



/*
void 	start_motors	(int percent)
{
	LEFT_MOTOR_VALUE = MOTOR_PWM_MOD / 8;//factorul de umplere de 50%
	RIGHT_MOTOR_VALUE= MOTOR_PWM_MOD / 8;//factorul de umplere de 20%
}

void 	turn			(int angle)
{
	//transforma cumva unghiul in factor de umplere!!!
	SERVO_MOTOR_VALUE= SERVO_PWM_MOD - 10;
}

void 	turn_left		(int angle)
{
	turn (-angle);
}

void 	turn_right		(int angle)
{
	turn (angle);
}
*/