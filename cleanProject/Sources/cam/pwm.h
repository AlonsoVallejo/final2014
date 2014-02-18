/*
 * pwm.h
 *
 *  Created on: 14.01.2014
 *      Author: Iulian
 */

#ifndef PWM_H_
#define PWM_H_
#include "derivative.h"

/*SECTION signal actions*/ //Semnal care spune directia pe care se deplaseza masina
#define SELECTION_MASK		(1 << 26)
#define SELECTION_HIGH		GPIOE_PSOR = SELECTION_MASK;
#define SELECTION_LOW		GPIOE_PCOR = SELECTION_MASK;

/*ENDL signal actions*/		//enable pentru motor stanga
#define ENDL_MASK			(1 << 14)
#define ENDL_HIGH			GPIOD_PSOR = ENDL_MASK;
#define ENDL_LOW			GPIOD_PCOR = ENDL_MASK;

/*ENDR signal actions*/		//enable pentru motor dreapta
#define ENDR_MASK			(1 << 3)
#define ENDR_HIGH			GPIOB_PSOR = ENDR_MASK;
#define ENDR_LOW			GPIOB_PCOR = ENDR_MASK;

#define M0VE_FORWARD()		SELECTION_LOW
#define MOVE_BACKWARD()		SELECTION_HIGH

#define ENABLE_MOTORS()		ENDR_HIGH; ENDL_HIGH;
#define DISABLE_MOTORS()	ENRR_LOW;  ENDL_LOW;

#define LEFT_MOTOR_VALUE 	FTM1_C1V
#define RIGHT_MOTOR_VALUE	FTM1_C0V
#define SERVO_MOTOR_VALUE	FTM0_C1V

#define THRESHOLD_ZERO 		40 

#define MOTOR_PWM_MOD		0x1000
#define SERVO_PWM_MOD		0x7A12

#define MAX_PWM 			256//nr de trepte pwm motoare
#define MAX_SERVO 			64//nr de trepte pwm servo-motor

#define SERVO_MIN_PWM 		1130
#define SERVO_MAX_PWM 		2230
#define SERVO_CENTER 		1723  //old value 1643 


#define SET_DUTY_LEFT(x) 	LEFT_MOTOR_VALUE  = ((x+THRESHOLD_ZERO)*(MOTOR_PWM_MOD/MAX_PWM))
#define SET_DUTY_RIGHT(x) 	RIGHT_MOTOR_VALUE = ((x+THRESHOLD_ZERO)*(MOTOR_PWM_MOD/MAX_PWM))

#define SET_SERVO_CENTER() 	SERVO_MOTOR_VALUE = SERVO_CENTER

//obsolete
#define SERVO_CENTER_PWM 	1630 //1658, 1675 ok
#define SERVO_RANGE 		1000 // (SERVO_MAX_PWM-SERVO_MIN_PWM)/2
#define SET_POS_SERVO(x) 	SERVO_MOTOR_VALUE = SERVO_CENTER_PWM + x*(SERVO_RANGE)/127

/*		methods		*/
void SET_SERVO_LEFT		(int x);
void SET_SERVO_RIGHT	(int x);

void init_servo			(void);//servo motor

void init_motors		(void);//left motor,right motor

void init_motor_signals	(void);//selection, endl and endr

void init_pwm			(void);

#endif /* PWM_H_ */
