/*
 * utils.c
 *
 *  Created on: Feb 1, 2013
 *      Author: nexz
 */

#include "common.h"
#include "utils.h"
#include "pwm.h"
#include "control.h"

static int servo;
unsigned char c;
int stopped;
extern int count_pit0;
int demo_state = 0;
int follow_line = 1;
extern int DEFAULT_STEP;
extern int command;

void check_cmds() {
	if (char_present()) {
		c = in_char();
		GPIOC_PTOR = GPIO_PIN(7);

			switch (c) {
			case 0:
				printf("\ngo\ngo\ngo\n");
				command = 0;
				do_accelerate(80);
				break;
			case 1:
				command = 1;
				printf("forward\n");
				set_direction(1);
				start_chspeed(MS_TO_CLOCKS(1000), 80);
				break;
			case 2:
				command = 1;
				printf("log pentru turatii\n");
				set_direction(1);
				count_pit0 = 0;
				start_chspeed(MS_TO_CLOCKS(1000), 35);
				break;
			case 3:
				command = 3;
				break;
			case 4:
				command = 1;
				start_chspeed(MS_TO_CLOCKS(2000), 140);
				break;
			case 5:
				command = 2;
				printf("forward\n");
				set_direction(1);
				start_chspeed(MS_TO_CLOCKS(10), 80);
				break;
			case 6:
				command = 2;
				printf("stop\n");

				break;
			case 7:
				command = 2;
				printf("backward\n");
				set_direction(0);
				start_chspeed(MS_TO_CLOCKS(10), 175);
				break;
			case 8:
				start_chspeed(MS_TO_CLOCKS(5000), 9 * MAX_PWM / 10);
				break;
			case 9:
				start_chspeed(MS_TO_CLOCKS(5000), MAX_PWM);
				break;
			case 11:
				command = 6;
				count_pit0 = 0;
				start_chspeed(MS_TO_CLOCKS(1000), 80);
				break;
			case 12:
				command = 7;
				count_pit0 = 0;
				start_chspeed(MS_TO_CLOCKS(1000), 80);
				break;
			case 255:
				//send_log_ascii();
				break;
			case 101:
				follow_line = 1 - follow_line;
				break;
			}

			stopped = 0;
			demo_state = 0;
			//log_index = 0;
			//logging = 1;

		if (c > 20 && c < 210) {
			command = 1;
			printf("log pentru turatii\n");
			do_accelerate(c);
		}
	}

}
