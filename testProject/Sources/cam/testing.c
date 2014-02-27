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
int demo_state = 0;
int follow_line = 1;
//TO DO: adaugare cod PID turatie
int turatie_ref = 0;
extern int turatie_crt;
extern int DEFAULT_STEP;
extern int command;

//for testing servo
int servo_val;

void test(char cmd)
{
	SELECTION_LOW;
	enable_motors();
	
	switch (cmd)
	{
	case '1':
		//init_chspeed();
		start_chspeed(MS_TO_CLOCKS(1000), 60);
		break;
	case '2':
		start_chspeed(MS_TO_CLOCKS(1000), 80);
		break;
	case '3':
		start_chspeed(MS_TO_CLOCKS(1000), 100);
		break;
	case '4':
		start_chspeed(MS_TO_CLOCKS(1000), 120);
		break;
	case '5':
		start_chspeed(MS_TO_CLOCKS(2000), 0);
		break;
	case '6':
		disable_motors();
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
		
		

	}
}
