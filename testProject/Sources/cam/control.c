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

extern unsigned char *cam_prel;	
extern unsigned char *cam_prel;
extern unsigned char camera1_buff[128];
extern unsigned char camera2_buff[128];
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


#define A0 2
#define A1 -2

//first working pid
//calculated via matlab 
//change for fine tunning
#define N  2048 
#define  Ki (N*0)
#define  Kp  (2*N) 
#define Kd  (1*0 )
//must remain constant
const int  a0 =Kp + Ki/200+Kd*200;
const int a1 =(-Kp)-2*Kd*200;
const int a2 =(Kd)*200;  



double PID(int servo);
double P(double x);
double I();
double D();
double derivative();
int minus = 0;

//added for testing servo
extern int servo_val;
//contor turatie
extern int counter_turatie;

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


//bigcount for plotting curve 
extern uint32 bigcount;
void set_steering_position();
int u=0;

int need_brake=0;
const int sum_max=(LINE_BUF_MAX*(LINE_BUF_MAX-1)/2);
int  line_buf[LINE_BUF_MAX]={0};
int head =0 ; 
int show_it = 0;
int state = 0  ; 

extern unsigned char turatie_ref;
extern char velocity_state;
extern int pwm_crt;
int prevStartTime = 0; //timpul de aparitie al frame-ului anterior de start
int firstLineDetection = 1; // variabila pentru pornirea masinutei daca prima oara se detecteaza frame de linie si nu de start
int startCounter = 0; //pentru contorizarea frame-urilor de start

//for testing pid 
int prev_err = 0 ; 


//pentru curba automata
int servo_center_detected=0;
int servo_curve_detected = 0;
unsigned char in_curve = 0;


void check_monotony()
{
	int i = 0 ;
	int sign_count = 0 ; 
	int current = head;
	int head_sign;
	int previous;
	int diff=0;
	previous = ((head-DIFFERENCE)%LINE_BUF_MAX+LINE_BUF_MAX)%LINE_BUF_MAX;
	head_sign = line_buf[head] / abs(line_buf[head]);
	
	for (;i<SAMPLE_SIZE;i++)
	{
		//~|~|_|_|_|_|_
		diff = (line_buf[current]-line_buf[previous]);
		if (diff!=0)
		{
			sign_count += (( diff/ abs(diff))*head_sign + 1)/2;
		}
		
		current = previous;
		previous = ((previous-DIFFERENCE)%LINE_BUF_MAX+LINE_BUF_MAX)%LINE_BUF_MAX;	
	}
	
	//io_printf("#%d#\n",sign_count);
	if((sign_count * 100)/SAMPLE_SIZE < THRESHOLD_ACC && state == 1 )
		{
			//io_printf("out\n");
			start_chspeed(MS_TO_CLOCKS(200),70);
			state = 0 ; 
		}
	
	if((sign_count * 100)/SAMPLE_SIZE > THRESHOLD_BRAKE && state == 0)
		{
			//io_printf("in\n");
		start_chspeed(MS_TO_CLOCKS(200),30);
			state = 1 ; 
		}
}

int avg()
{
	int i = 0 ;
	int sum=0  ;
	int start = head ; 
	int count = LINE_BUF_MAX ; 
	int to_print;
	for (;i<LINE_BUF_MAX;i++)
	{
		sum+=line_buf[start];//*count;
		count--;
		//_|_|_|_|_|_|_
		start = ((start -1)%LINE_BUF_MAX+LINE_BUF_MAX)%LINE_BUF_MAX;
	}
	 to_print=  sum/LINE_BUF_MAX;//sum_max;
	
	return to_print;
}

int avg_china()
{
	int i = 0 ;
	int sum=0  ;
	int start = head ; 
	int count = LINE_BUF_MAX ; 
	int to_print;
	for (;i<LINE_BUF_MAX;i++)
	{
		sum+=line_buf[start]*count;
		count--;
		//_|_|_|_|_|_|_
		start = ((start -1)%LINE_BUF_MAX+LINE_BUF_MAX)%LINE_BUF_MAX;
	}
	 to_print=  sum/sum_max;
	
	return to_print;
}

void Pi()
{
	int head1,line_buf1[128];
	int error=0;
	int i;
	int u1=0;
	unsigned long int time = 0;
	
	for(i=0;i<32;i++)
	{
		line_buf1[i]=0;
		line_buf1[i+32]=10;
		line_buf1[i+64]=0;
		line_buf1[i+96]=-10;
	}
	
	for(head1=1;head1<128;head1++)
	{
		//(abs(line_buf[head])/line_buf[head])*
	error = ((21*line_buf1[head1]) - (20*line_buf1[((head1 -1)%128+128)%128]))/10;
	u1+=error;
	for(time =0; time < 2000000;time++)
	{
		asm("nop");
	}
	io_printf("%d %d %d\n",error,u1,line_buf1[head1]);
	}
}


void inline  my_PID ()
{
	//io_printf("%d %d %d #\n",a0,a1,a2);
	u+= (a0*line_buf[head]+ a1*line_buf[((head -1)%LINE_BUF_MAX+LINE_BUF_MAX)%LINE_BUF_MAX]+
			a2*line_buf[((head -2)%LINE_BUF_MAX+LINE_BUF_MAX)%LINE_BUF_MAX])/N;
}

void update_track_info()
{
	my_PID ();
	crnt_frame.linepos = u ; 
	

	if(abs(crnt_frame.linepos)>15)
	{
		if(in_curve == 0)
			servo_curve_detected++;
		servo_center_detected=0;
		if(servo_curve_detected == 10)
		{
			velocity_state = BRAKE_STAGE0;
			in_curve = 1;
			io_printf("c\n");
			servo_curve_detected++;
		}
	}
	else
	{
		if(in_curve == 1)
			servo_center_detected++;
		servo_curve_detected=0;
		if(servo_center_detected == 20)
		{
			//velocity_state = ACCELERATE;
			turatie_ref = 20;
			in_curve=0;
			io_printf("l\n");
			servo_center_detected++;
		}
	}
}

void got_frame() {
	
	int i, edgei = 0;
	char c = 255;
	int error=0;
	

	// first update time
	time_5ms++;
	time_50ms++;
	
	/*time_2s_aux++;
	if (time_2s_aux == 200) {
		time_2s_aux = 0;
		sw++;
		if (sw == 4) {
			sw = 0;
		}
	}

	count_pit0++;*/

	//detect edges in frame using a threshold of the derivative
	for (i = MARGIN + OFFSET; i < 128 - MARGIN; i += 1) {
		dc = cam_prel[i] - cam_prel[i - OFFSET];
		
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

			//worked at freescale 
			//crnt_frame.linepos = ((line_buf[head]*K1)/P1)+(((line_buf[head]-avg())*K2)/P2);
			//crnt_frame.linepos += line_buf[head]-avg_china();
			//newly developed modifies u 			
				
				/*if(show_it == 10)
				{
					check_monotony();
					show_it = 0;
				}
				show_it++;
				*/
			line_buf[head]= linepos - CENTER_CORRECTION;
			update_track_info();
			our_set_steering_position();
			head=(head+1)%LINE_BUF_MAX;
						//+((linepos-CENTER_CORRECTION)/abs(linepos-CENTER_CORRECTION))*CURVE_OFFSET;
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
		break;
	case FRAME_ERROR:
		last_error_time = time_5ms;
		//io_printf("e\n");
		break;
	case FRAME_START:
		//last_start_time = time_5ms;
		//io_printf("s\n");
		//daca nu mai capturasem niciun frame de start
		/*if(time_5ms - prevStartTime > 7)
		{
			startCounter++;
		//	io_printf("s:%d - %d - %d\n", startCounter, time_5ms, prevStartTime);
			prevStartTime = time_5ms;
			
			if(startCounter == 2)
			{
				//disable_motors();
				turatie_ref = 0;
				pwm_crt=0;
				
				//pentru testare (sa se poata lua de la capat fara a trebui reset)
				startCounter = 0;
			}
		}*/
		break;
	case FRAME_LINE:
		//line_buf[head]= linepos - CENTER_CORRECTION; 
		//head=(head+1)%LINE_BUF_MAX;
		last_line_time = time_5ms;
		break;
	}

	// add frame info to ring buffer (to be used later)
	//RBUFF_PUSH(crnt_frame);

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
	/*

	 #ifdef LOG_FRAME_DATA
	 if (LOG_NOT_FULL() && logging) {
	 GPIOC_PCOR |= GPIO_PIN(9);

	 add_log_entry(&entry);

	 }
	 else		GPIOC_PSOR |= GPIO_PIN(9);
	 #endif
	 */

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

int our_set_steering_position()
{
	int x;
	line_error_val = crnt_frame.linepos;
		if (crnt_frame.linepos < 0 /*&& CURVE_THRESHOLD < line_error_val*/) {
			servo =  crnt_frame.linepos;
			SET_SERVO_LEFT(-servo);
		} else if (crnt_frame.linepos > 0 /*&& CURVE_THRESHOLD < line_error_val*/) {
			servo =  crnt_frame.linepos;
			SET_SERVO_RIGHT(servo);
		}
		prev_error = line_error_val;
	
		return 0;
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
		our_set_steering_position();
		if (count_pit0 > 25 && state == 0) {
			set_direction(0);
			minus = 1;
			start_chspeed(MS_TO_CLOCKS(BRK_FALLING_TIME), get_reference() / 2);
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
	if (get_spd() > 100)
		while (state < 5) {
			set_steering_position();
			if (count_pit0 > 25 && state == 0) {
				set_direction(0);
				minus = 1;
				start_chspeed(MS_TO_CLOCKS(BRK_FALLING_TIME), get_spd() / 4);
				count_pit0 = 0;
				//	printf("going to -50\n");
				state = 1;
			} else if (count_pit0 > BRK_FIRST_VALUE_TIME / 5 && state == 1) {
				start_chspeed(MS_TO_CLOCKS(BRK_RISING_TIME), 0);
				//	printf("going to 0\n");
				count_pit0 = 0;
				state = 2;
			} else if (count_pit0 > BRK_FIRST_VALUE_TIME / 5 && state == 2) {
				start_chspeed(MS_TO_CLOCKS(BRK_FALLING_TIME), get_spd() / 4);
				//	printf("going to 0\n");
				count_pit0 = 0;
				state = 3;
			} else if (count_pit0 > BRK_FIRST_VALUE_TIME / 5 && state == 3) {
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

void set_PWM(int pwm) {
	//printf("%d -> %d\n",get_reference(),pwm);
	if (get_reference() == pwm)
		return;
	if (pwm > get_reference()) {
		do_accelerate(pwm);
	} else if (pwm < get_reference()) {
		do_brake(pwm);
	}
}
