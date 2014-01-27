/*
 * motors.h
 *
 *  Created on: 16.01.2014
 *      Author: Iulian
 */

#ifndef MOTORS_H_
#define MOTORS_H_

#include "derivative.h"
#include "pwm.h"
#include "gpio.h"

/*
#define MOVE_FORWARD 	SELECTION_ON;
#define MOVE_BACKWARD	SELECTION_OFF;

#define LEFT_MOTOR_ON	ENDL_ON;
#define	LEFT_MOTOR_OFF	ENDL_OFF;

#define RIGHT_MOTOR_ON	ENDR_ON;
#define RIGHT_MOTOR_OFF	ENDR_OFF;

void 	start_motors 	(int percent);

void 	turn_left		(int angle);

void 	turn_right		(int angle);
*/

#define MIN_CLOCKS_PER_DIFFERENCE_UP   10000
#define MIN_CLOCKS_PER_DIFFERENCE_DOWN 10000

#define NOT_CHSPEED_AT(ref) (ref!=get_reference()) 

void stop_chspeed();
void set_direction(int forw);
void start_chspeed(unsigned int clocks,short int difference);
void disable_motors();
void enable_motors();
const unsigned char get_spd();
const unsigned char get_reference();
#endif /* MOTORS_H_ */
