/*
 * testing.c
 *
 *  Created on: 01.02.2014
 *      Author: Iulian
 */

#include "testing.h"
static int servo;

int stopped;
extern int count_pit0;
extern int brake;
int demo_state = 0;
int follow_line = 1;
//TO DO: adaugare cod PID turatie


extern int DEFAULT_STEP;
extern int command;
extern unsigned char turatie_ref;
extern int pwm_crt;
//for testing servo
int servo_val;

void test(char cmd)
{
	SELECTION_LOW;
	enable_motors();
	
	switch (cmd)
	{
	case '1':
		turatie_ref = 5;
		break;
	case '2':
		turatie_ref = 10;
		break;
	case '3':
		turatie_ref = 20;
		break;
	case '4':
		turatie_ref = 30;
		break;
	case '5':
		turatie_ref = 40;
		break;
	case '6':
		disable_motors();
		turatie_ref = 0;
		pwm_crt=0;
		break;
	case '7':
		out_char('7');
		SELECTION_HIGH;//INAPOI
		break;
	case 'd':
		SET_SERVO_RIGHT(20);
		break;
	case 'c':
		SET_SERVO_CENTER();
		break;
	case 'a':
		servo_val-=10;
		io_printf("%d\n",servo_val);
		our_set_steering_position();
		break;
	case 's':
		servo_val+=10;
		io_printf("%d\n",servo_val);
		our_set_steering_position();
		break;
	case 'b':
		brake = (brake+1)%2;
		break;
		
		

	}
}
