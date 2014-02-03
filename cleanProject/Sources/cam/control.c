/*
 * sensors.c
 *
 *  Created on: Jan 19, 2013
 *      Author: nexz
 */
#define IN_CONTROL_C

#include "math.h"
#include "control.h"
#include "utils.h"
#include "pwm.h"
#include "motors.h"

extern short int forward; //and 0 brake
extern int line_error_val;
#define CRNT_FRAME RBUFF_SIZE-1
// NEWMOTORS
int adc_read_left_signed_minus_1 = 0;
int adc_read_right_signed_minus_1 = 0;
extern short int reference;
#define ACC_RISING_TIME 		100
#define ACC_FIRST_VALUE			100
#define ACC_FIRST_VALUE_TIME 	10	
#define ACC_FALLING_TIME		100
#define ACC_SECOND_VALUE		ACC_FIRST_VALUE - 25

#define BRK_FALLING_TIME 		ref/4
#define BRK_FIRST_VALUE_TIME 	0			
#define BRK_RISING_TIME			ref/4


#define Kp_coef_A 3 
#define Kp_coef_B 2
#define Ki_coef_A 1
#define Ki_coef_B 120	//80 //120
#define Kd_coef_A 1
#define Kd_coef_B 1

#define Kp(x) ((x*Kp_coef_A)/Kp_coef_B)
#define Ki() ((integral()*Ki_coef_A)/Ki_coef_B)
#define Kd() ((derivative()*Kd_coef_A)/Kd_coef_B)/// derivative trebuie implementat
double PID(int servo);
double P(double x);
double I();
double D();
double derivative();
int minus = 0;
extern int brake;
extern int main_error;
extern RBUFF_TYPE rbuff[RBUFF_SIZE];
extern int prev_error;
extern int count_pit0;
static int adc_read_state = 1;
unsigned char adc_read_left = 0;
unsigned char adc_read_right = 0;
short adc_read_left_signed = 0;
short adc_read_right_signed = 0;
int time_50ms;
extern int bigcount;
extern int sw;
extern int command;

log_entry entry;

float k_left = 0;
float k_right = 0;
float W_left;
float W_right;
float PWML = 0;
float PWMR = 0;
int PWML_ABS = 0;
int PWMR_ABS = 0;
float ref_R;
float ref_L;
float ref_R_minus_1 = 0;
float ref_L_minus_1 = 0;

int time_2s_aux = 0;
// SENSORS
static int counter = 0;
static int8 dc, sign;
int aux_pwm;
static int8 edge_pos[16];
static int8 edge_type[16];
static int8 code = 0;
static int8 linepos;
static int8 linewidth;

current_read fb;

void adc0_current_read();
extern int flag_sec;
extern int servo;

void set_steering_position();

void gen_PWM_L(int ref) {
	ref_L = ref - W_left;
	PWML = ref_L * 4.014 - 4 * ref_L_minus_1 + PWML;
	ref_L_minus_1 = ref_L;

}

void gen_PWM_R(int ref) {
	ref_R = ref - W_right;
	PWMR = ref_R * 4.014 - 4 * ref_R_minus_1 + PWMR;
	ref_R_minus_1 = ref_R;

}

void gen_PWM(int pwm) {
	gen_PWM_L(pwm);
	gen_PWM_R(pwm);
}

void gen_W_k_left() {
	W_left = 0.9984 * k_left + (0.003 * adc_read_left_signed);
	k_left = W_left;
}

void gen_W_k_right() {
	W_right = 0.9984 * k_right + (0.003 * adc_read_right_signed);
	k_right = W_right;
}

void gen_Ws() {
	gen_W_k_left();
	gen_W_k_right();
}

void got_frame() {
	int i, edgei = 0;
	char c = 255;
	// first update time
	time_5ms++;
	time_50ms++;
	time_2s_aux++;
	if (time_2s_aux == 200) {
		time_2s_aux = 0;
		sw++;
		if (sw == 4) {
			sw = 0;
		}

	}
	
	count_pit0++;
	if (time_50ms == 200) {
		c = 255;
		out_char(c);
		US_TO_CLOCKS(400);
		c = 0;
		out_char(c);
		US_TO_CLOCKS(400);
		c = 255;
		out_char(c);
		US_TO_CLOCKS(400);
		c = 0;
		out_char(c);
		US_TO_CLOCKS(400);
	}
	
	set_steering_position();
	
	//detect edges in frame using a threshold of the derivative
	for (i = MARGIN + OFFSET; i < 128 - MARGIN; i += 1) {
		dc = cam_buff[i] - cam_buff[i - OFFSET];
		//may optimise ringbuff with shift
		if (time_50ms == 200) {
			out_char(cam_buff[i]);
			US_TO_CLOCKS(400);
		}
		sign = 1;
		if (dc < 0) {
			dc = -dc;
			sign = -1;
		}
		if (dc > DEFAULT_LINE_THRESHOLD) {
			edge_pos[edgei] = i;
			edge_type[edgei] = sign;
			edgei += 1;
			i += OFFSET;
			if (edgei >= 16)
				break;
		}

	}
	if (time_50ms == 20) {
		time_50ms = 0;

	}

	// proceed to interpreting the edge list
	crnt_frame.type = FRAME_NONE;
	crnt_frame.linepos = 0;
	code = 0; // 1 - up 2 -down 3 -up 4 (edges)
	for (i = 1; i < edgei; i += 1)
		if (edge_type[i - 1] < 0 && edge_type[i] > 0) {
			linepos = (edge_pos[i] + edge_pos[i - 1]) / 2 - 64
					- OFFSET_CORRECTION;
			linewidth = edge_pos[i] - edge_pos[i - 1];
			if (linewidth > LINE_MIN_WIDTH && linewidth < LINE_MAX_WIDTH) {
				if (crnt_frame.type == FRAME_NONE) // check if it is the first line of the desired width 
					crnt_frame.type = FRAME_LINE;
				else
					crnt_frame.type = FRAME_ERROR; // if not its a malformed frame
				crnt_frame.linepos = linepos;
				if (code == 1)
					code = 2;
				else
					code = 0;
			}
		} else if (edge_type[i - 1] > 0 && edge_type[i] < 0) {
			linewidth = edge_pos[i] - edge_pos[i - 1];
			if (linewidth > MIN_START_GAP && linewidth < MAX_START_GAP) {
				if (code == 2)
					crnt_frame.type = FRAME_START;
				code = 1;
			}
		}

	switch (crnt_frame.type) {
	case FRAME_NONE:
	case FRAME_ERROR:
		last_error_time = time_5ms;
		break;
	case FRAME_START:
		last_start_time = time_5ms;
		break;
	case FRAME_LINE:
		last_line_time = time_5ms;
		break;
	}

	// add frame info to ring buffer (to be used later)
	RBUFF_PUSH(crnt_frame);

	ADC0_SC1A = ADC0_SC1A_VAL_LEFT;
	adc_read_state = 1;

	fb.time = time_5ms;
	fb.left = adc_read_left_signed;
	fb.right = adc_read_right_signed;

	fb.pwm = get_spd();
	fb.W_left = W_left;
	fb.W_right = W_right;

	if (minus == 1) {
		short aux = fb.pwm;
		entry.pwm = -(aux + 40);
	} else {
		entry.pwm = fb.pwm + 40;
	}

	entry.right = fb.right;

#ifdef LOG_FRAME_DATA
	if (LOG_NOT_FULL() && logging) {
		GPIOC_PCOR |= GPIO_PIN(9);

		add_log_entry(&entry);

	}
else		GPIOC_PSOR |= GPIO_PIN(9);
#endif

	}

// part copied from newmotors.h
void motors_adc0_isr() {

	adc_read_right_signed_minus_1 = adc_read_right_signed;
	adc_read_left_signed_minus_1 = adc_read_left_signed;
	if (adc_read_state == 1) {
		adc_read_left = ADC0_RA;
		ADC0_SC1A = ADC0_SC1A_VAL_RIGHT;
		adc_read_state = 2;
	} else {
		adc_read_right = ADC0_RA;
	}

	adc_read_left_signed = (short) adc_read_left;
	adc_read_right_signed = (short) adc_read_right;
	if (minus == 1) {
		adc_read_left_signed = -adc_read_left_signed;
		adc_read_right_signed = -adc_read_right_signed;
	}
}

float integral() {
	float ret_val = 0;
	int i = 0;
	RBUFF_LOOP(i) {
		if (rbuff[i].type == FRAME_LINE)
			ret_val += rbuff[i].linepos;
		RBUFF_NEXT(i);
	}
	return ret_val;
}

double derivative() {
	double ret_val;
	ret_val = line_error_val - prev_error;
}

double P(double x) {
	return ((x * Kp_coef_A) / Kp_coef_B);
}

double I() {
	return ((integral() * Ki_coef_A) / Ki_coef_B);
}

double D() {
	return ((derivative() * Kd_coef_A) / Kd_coef_B);
}

double PID(int servo) {
	return P(servo) + I() + D();
}

void set_steering_position() {
	line_error_val = crnt_frame.linepos;
	if (crnt_frame.linepos < 0/* && CURVE_THRESHOLD < line_error_val*/) {
		servo = crnt_frame.linepos;
		SET_SERVO_LEFT(-(int) PID(servo));
	} else if (crnt_frame.linepos > 0/* && CURVE_THRESHOLD < line_error_val*/) {
		servo = crnt_frame.linepos;
		SET_SERVO_RIGHT((int) PID(servo));
	}
	prev_error = line_error_val;
}

void do_accelerate(int PWM) {
	int state = 0;
	count_pit0 = 0;
	//printf("Acc from %d to %d",get_spd(),PWM);
	out_char('a');
	while (state < 2) {
		out_char('b');
		set_steering_position();
		if (state == 0) {
			out_char('c');
			set_direction(1);
			start_chspeed(MS_TO_CLOCKS(ACC_RISING_TIME), PWM);
			state = 1;
			count_pit0 = 0;
		} else if (state == 1 && count_pit0 > ACC_FIRST_VALUE_TIME / 5) {
			out_char('d');
			start_chspeed(MS_TO_CLOCKS(ACC_FALLING_TIME), PWM);
			state = 2;
		}
	}
	count_pit0 = 0;
}

void do_brake_to_0() {
	int state = 0;
	int ref = get_reference();
	count_pit0 = 0;
	
	//printf("brake to 0\n");
	while (state < 2) {
		set_steering_position();
		if (count_pit0 > 25 && state == 0) {
			set_direction(0);
			minus = 1;
			start_chspeed(MS_TO_CLOCKS(BRK_FALLING_TIME), get_reference()/2);
			count_pit0 = 0;
		//	printf("going to -65\n");
			state = 1;
		} else if (count_pit0 > BRK_FIRST_VALUE_TIME / 5 && state == 1) {
			start_chspeed(MS_TO_CLOCKS(BRK_RISING_TIME), 0);
		//	printf("going to 0\n");
			state = 2;
		}
	}
	dumb_sleep(MS_TO_CLOCKS(BRK_RISING_TIME));
	disable_motors();
	SET_DUTY_LEFT(-40);
	SET_DUTY_RIGHT(-40);
}

void do_brake(int PWM) {
	int state = 0;
	int ref = get_reference();
	count_pit0 = 0;
	//printf("brake to %d\n", PWM);
	if(get_spd() > 100)
	while (state < 5) {
		set_steering_position();
		if (count_pit0 > 25 && state == 0) {
			set_direction(0);
			minus = 1;
			start_chspeed(MS_TO_CLOCKS(BRK_FALLING_TIME), get_spd()/4);
			count_pit0 = 0;
		//	printf("going to -50\n");
			state = 1;
		} else if (count_pit0 > BRK_FIRST_VALUE_TIME/5  && state == 1) {
			start_chspeed(MS_TO_CLOCKS(BRK_RISING_TIME), 0);
		//	printf("going to 0\n");
			count_pit0 = 0;
			state = 2;
		} else if (count_pit0 > BRK_FIRST_VALUE_TIME/5  && state == 2) {
			start_chspeed(MS_TO_CLOCKS(BRK_FALLING_TIME), get_spd()/4);
		//	printf("going to 0\n");
			count_pit0 = 0;
			state = 3;
		} else if (count_pit0 > BRK_FIRST_VALUE_TIME/5  && state == 3) {
			start_chspeed(MS_TO_CLOCKS(BRK_RISING_TIME/2), 0);
		//	printf("going to 0\n");
			count_pit0 = 0;
			state = 4;
		} else if (count_pit0 > BRK_RISING_TIME / 10 && state == 4) {
			set_direction(1);
			start_chspeed(MS_TO_CLOCKS( BRK_RISING_TIME/2 ), PWM);
		//	printf("going to %d\n",PWM);
			state = 5;
		}
	}
	//dumb_sleep(MS_TO_CLOCKS(BRK_RISING_TIME));
}

void set_PWM(int pwm){
	//printf("%d -> %d\n",get_reference(),pwm);
	if(get_reference() == pwm)return;
	if(pwm > get_reference()){
		do_accelerate(pwm);
	}else if(pwm < get_reference()){
		do_brake(pwm);
	}
}