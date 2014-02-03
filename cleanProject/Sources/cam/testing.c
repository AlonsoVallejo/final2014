/*
 * testing.c
 *
 *  Created on: 01.02.2014
 *      Author: Iulian
 */

#include "testing.h"

void test(char cmd)
{
	SELECTION_LOW;
	enable_motors();
	switch (cmd)
	{
	case '1':
		out_char('1');
		init_chspeed();
		start_chspeed(MS_TO_CLOCKS(2000), 25);
		out_char('1');
		break;
	case '2':
		out_char('2');
		start_chspeed(MS_TO_CLOCKS(3000), 80);
		out_char('2');
		break;
	case '3':
		enable_motors();
		break;
	case '4':
		disable_motors();
		break;
	case '5':
		start_chspeed(MS_TO_CLOCKS(4000), 0);
		break;
	case '6':
		out_char('6');
		SELECTION_LOW;//INAINTE
		break;
	case '7':
		out_char('7');
		SELECTION_HIGH;//INAPOI
		break;

	}
}