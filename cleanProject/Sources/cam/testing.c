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
extern int DEFAULT_STEP;
extern int command;

void test(char cmd)
{
	SELECTION_LOW;
	enable_motors();
	//int servo_irina=0;
	//int servo_hristea=0;
	switch (cmd)
	{
	case '1':
		init_chspeed();
		start_chspeed(MS_TO_CLOCKS(2000), 32);
		break;
	case '2':
		start_chspeed(MS_TO_CLOCKS(3000), 80);
		break;
	case '3':
		enable_motors();
		break;
	case '4':
		disable_motors();
		break;
	case '5':
		start_chspeed(MS_TO_CLOCKS(2000), 0);
		break;
	case '6':
		out_char('6');
		SELECTION_LOW;//INAINTE
		break;
	case '7':
		out_char('7');
		SELECTION_HIGH;//INAPOI
		break;
	case 's':
		//servo_irina+=10;
		SET_SERVO_LEFT(20);
		break;
	case 'd':
		SET_SERVO_RIGHT(20);
		break;
	case 'c':
		SET_SERVO_CENTER();
		break;

	}
}