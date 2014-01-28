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
