/*
 * pwm.c
 *
 *  Created on: 14.01.2014
 *      Author: Iulian
 */

#include "derivative.h"
#include "pwm.h"



void SET_SERVO_LEFT(int x) {

	x = SERVO_CENTER - (SERVO_CENTER - SERVO_MIN_PWM) * x / MAX_SERVO;
	if (x < 1120) {// sa nu fortez prea tare stanga
		x = 1120;
	}
	SERVO_MOTOR_VALUE = x;
}

void SET_SERVO_RIGHT(int x) {
	x = SERVO_CENTER + (SERVO_MAX_PWM - SERVO_CENTER) * x / MAX_SERVO;
	if (x > 2240) {//sa nu fortez prea tare dreapta
		x = 2240;
	}
	SERVO_MOTOR_VALUE = x;
}

void init_pwm		(void)
{
	init_motors();
	init_servo();
	init_motor_signals();
}

void init_motors	(void)
{
		/*
		 * each FMT module has 8 channels.
		 * we will use 2 of them for the 2 motors
		 */
	//enable portb clock
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	//select pin functionality (FTM for PWM)
	PORTB_PCR1 = PORT_PCR_MUX(0x3); //// FTM1_CH1 - PIN 5
	PORTB_PCR0 = PORT_PCR_MUX(0x3); //// FTM1_CH0 - PIN 3
	
	//enable FMT1 module
	SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;
		
	//disable write protection - pentru a putea scrie anumiti registri specifici modulului de ftm ( QDCTRL , DECAPEN etc.)
	FTM1_MODE = FTM_MODE_WPDIS_MASK;
	
	//enable all FTM specific registers
	FTM1_MODE &= ~(FTM_MODE_FTMEN_MASK);
	
	//disable FTM1 module - pentru a fi siguri ca scriem registrii faca nici o restrictie
	FTM1_SC = 0;
	
	//dupa ce registrul FTMx_CNT ajunge la valoarea din FTMx_MOD va incepe noua numaratoare de la FMTx_CNTIN
	FTM1_CNTIN = 0x00;	
	
	//ftm counter - aici se incrementeaza la fiecare semnal ftm clock ( semnalul de ceas dupa ce trece de ftm prescaler)
	FTM1_CNT   = 0x00;
	
	//modulo - pana la ce valoare se incrementeaza counter-ul.
	//Cand acesta ajunge la valoare mod, se va genera intrerupere, iar valoare de pe canalul n va fi fortata pe 0
	FTM1_MOD = MOTOR_PWM_MOD;

	//disable quadrature decoder - pentru modul edge-aligned avem nevoie ca acest registru sa fie 0!!!
	FTM1_QDCTRL = 0;
	
	//set channel 0 to: edge aligned PWM and high true pulses
	FTM1_C1SC = FTM_CnSC_ELSB_MASK | FTM_CnSC_MSB_MASK;	
	
	//set channel 1 to: edge aligned PWM and high true pulses
	FTM1_C0SC = FTM_CnSC_ELSB_MASK | FTM_CnSC_MSB_MASK;
	
	//pentru a seta care sa fie factorul de umplere pentru fiecare canal, vom seta registrul CnV
	FTM1_C0V = 0;
	FTM1_C1V = 0;
	
	//select the system clock for FTM1 module
	FTM1_SC = FTM_SC_CLKS(1);
}

void init_servo		(void)
{
	//pentru a intelege scopul fiecarei instructiuni, puteti citi comentariile din functia de mai sus!!!
	
	//enable port C clock
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	//seteaza functionalitatea de modul FTM si face enable pentru pinul 2 de pe portul C
	PORTC_PCR2 = PORT_PCR_MUX(0x4); //// FTM0_CH1 - PIN 11
	
	//enable pentru modulu de ftm (FTM0)
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;
	  
	FTM0_MODE |= FTM_MODE_WPDIS_MASK;
	
	FTM0_MODE &= ~(FTM_MODE_FTMEN_MASK);
	
	FTM0_SC = 0;
	
	FTM0_CNTIN = 0x00;
	  
	FTM0_MOD = SERVO_PWM_MOD;
	
	FTM0_C1SC = FTM_CnSC_ELSB_MASK | FTM_CnSC_MSB_MASK;
	 	
	FTM0_QDCTRL = 0;
	
	FTM0_C1V = SERVO_CENTER;

	FTM0_SC |= FTM_SC_CLKS(1)|FTM_SC_PS(5);
}

void init_motor_signals	(void)
{
	//enable clocks for switches and leds
	SIM_SCGC5 = SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK;
	
	//set gpio functionality
	PORTD_PCR14 = PORT_PCR_MUX(1);//ENDL
	PORTB_PCR3  = PORT_PCR_MUX(1);//ENDR
	PORTE_PCR26 = PORT_PCR_MUX(1);//SELECTION
	
	// Set direction of each pin - outputs/inputs
	GPIOD_PDDR |= ENDL_MASK;
	GPIOB_PDDR |= ENDR_MASK;
	GPIOE_PDDR |= SELECTION_MASK;
	
	// seteaza toate semnalele cu valoare initiala 0
	LEFT_MOTOR_VALUE = 0x00;
	RIGHT_MOTOR_VALUE= 0x00;
	SERVO_MOTOR_VALUE= 0x00;
	
}
