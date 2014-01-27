/*
 * pwm.h
 *
 *  Created on: 14.01.2014
 *      Author: Iulian
 */

#ifndef PWM_H_
#define PWM_H_

#include "gpio.h"
#define LEFT_MOTOR_VALUE 	FTM1_C1V
#define RIGHT_MOTOR_VALUE	FTM1_C0V
#define SERVO_MOTOR_VALUE	FTM0_C1V

#define THRESHOLD_ZERO 40 

#define MOTOR_PWM_MOD		0x1000
#define SERVO_PWM_MOD		0x7A12

#define MAX_PWM 256
#define SERVO_MIN_PWM 1130
#define SERVO_MAX_PWM 2230

#define MAX_SERVO 64
#define SERVO_CENTER 1643  


#define SET_DUTY_LEFT(x) LEFT_MOTOR_VALUE = ((x+THRESHOLD_ZERO)*(MOTOR_PWM_MOD/MAX_PWM))
#define SET_DUTY_RIGHT(x) RIGHT_MOTOR_VALUE = ((x+THRESHOLD_ZERO)*(MOTOR_PWM_MOD/MAX_PWM))

#define SET_SERVO_CENTER() SERVO_MOTOR_VALUE = SERVO_CENTER 

//obsolete
#define SERVO_CENTER_PWM 1630 //1658, 1675 ok
#define SERVO_RANGE 1000 // (SERVO_MAX_PWM-SERVO_MIN_PWM)/2
#define SET_POS_SERVO(x) SERVO_MOTOR_VALUE = SERVO_CENTER_PWM + x*(SERVO_RANGE)/127

void set_forward		();
void set_backward		();

void SET_SERVO_LEFT		(int x);
void SET_SERVO_RIGHT	(int x);

void init_servo			(void);//servo motor

void init_motors		(void);//left motor,right motor

void init_motor_signals	(void);//selection, endl and endr

void init_pwm			(void);

#endif /* PWM_H_ */
