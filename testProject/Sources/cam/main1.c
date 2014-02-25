/*
#include "derivative.h"
#include "utils.h"
#include "cam.h"
#include "pwm.h"
#include "testing.h"



// steering proportional cu factorul 3/2     /// 7/2
#define SHIFT_1 6
#define SHIFT_2 5
#define SHIFT_3 4
#define SHIFT_4 3
#define SHIFT_5 2
#define GEAR_1  70//80   //65
#define GEAR_2  75//100	//80
#define GEAR_3  80//115	//100
#define GEAR_4  100//135	//120
#define GEAR_5  120//155	//140
#define CRNT_FRAME RBUFF_SIZE-1
// valori de viteza in linie dreapta/curba -- FIX
#define STRAIGHT_SPEED (MAX_PWM/3)
#define CURVE_SPEED 65
// prag eroare linie pentru detectie curba -- FIX
#define CURVE_THRESHOLD 1






//definite in utils.c pentru comenzi
extern current_read fb;
extern int minus;
extern int stopped;
extern int demo_state;
extern int follow_line;
extern int count_pit0;
extern short adc_read_left_signed;
extern short adc_read_right_signed;



RBUFF_TYPE rbuff[RBUFF_SIZE];
int line_error;
int main_error = 0;
int direction = 1;
int brake = 0;
int last_error = 0, error = 0;
int line_error_val = 0;
int gear;
int sw;
int try_again = 0;
int gear_anterior = 0;
int prev_error = 0;
float servo;
int command = -1;
int st_challenge = 0;
int flag_sec;

double PID(int servo);
double P(double x);
double I();
double D();

uint32 bigcount=0;


void main(void) {
	//set oscilator clock to 50MHZ
	char c;
	MCG_FEI_BLPE();
	//initialize uart
	init_uart();
	//initialize general purpose i/o
	init_gpio();
	//initialize analog to digital converter
	init_adc();
	//other initializations used in this experiment
	init_cam();
	init_pwm();
	init_chspeed();
	
	out_char('s');
	while (1) {
		while(char_present()==0);
		c = in_char();
		out_char(c);
		test(c);
	}//While(1)
}//Main*/