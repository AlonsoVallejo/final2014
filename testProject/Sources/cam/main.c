#include "derivative.h"
#include "utils.h"
#include "cam.h"
#include "uart.h"
#include "pwm.h"
#include "motors.h"
#include "control.h"



int bigcount=-360;
RBUFF_TYPE rbuff[RBUFF_SIZE];

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
extern int counter_turatie;

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
unsigned char turatie_crt = 0;
int pwm_crt=0;
unsigned char turatie_ref=0;
extern unsigned char velocity_state;
unsigned char old_turatie_crt =0;
extern unsigned char config;

double PID(int servo);
double P(double x);
double I();
double D();

extern int need_brake;
extern int frincu;
char braking_persist = 0;
extern const unsigned char scenarios[3][3];

extern void update_track_info();

void main(void) {
	int i, abs_camerror, abs_camerror_before , speed ;
	int sending;//pentru a trimite frame
	int time_2s;
	int fps;
	char c;
	int time_5ms_passed = 0;
	abs_camerror_before=0;
	speed=80;
	fps = 50000 / (SI_TIMER / 1000);
	sending = 0;
	MCG_FEI_BLPE();
	
	
	init_uart();
	init_adc(); // setup ADC0 and ADC1, including ISR's from other source files
	init_cam(); // setup PIT0 and PIT1
	init_pwm(); // init ftm's for pwm generation - motors and servo
	init_gpio();
	
	init_chspeed(); // motors.c; until PID is available
	//direction: FORWARD
	SELECTION_LOW;
	//motors_enabled
	enable_motors();
	// center wheels on start
	SERVO_MOTOR_VALUE = SERVO_CENTER_PWM;
	stopped = 1;
	i=0;
	
	io_printf("b!!\n\r");

	while (1) {
		if(char_present())
		
			{c = in_char();
			test(c);
			
			}
		if(frincu)
		{
			got_frame();
			switch(velocity_state)
			{
			case BRAKE_STAGE0:
				SET_DUTY_LEFT(0);
				SET_DUTY_RIGHT(0);
				disable_motors();
				turatie_ref = scenarios[config][ACCL_INDEX];
				velocity_state = BRAKE_STAGE1;
				break;
			case BRAKE_STAGE1:
				enable_motors();
				MOVE_BACKWARD();
				SET_DUTY_LEFT(20);
				SET_DUTY_RIGHT(20);
				braking_persist++;
				if(braking_persist == scenarios[config][BRK_INDEX])//To Do: dependent de viteza
				{
					velocity_state = BRAKE_STAGE2;
					braking_persist = 0;
				}
				break;
			case BRAKE_STAGE2:
				disable_motors();
				pwm_crt = TURATIE_TO_PWM(turatie_ref+30);///TO DO: update +30 to something little more tech.
				enable_motors();
				M0VE_FORWARD();
				velocity_state = ACCELERATE;
				break;
			}
			frincu=0;
		}
		
		if (need_brake ==1 )
		{
			
				//start_chspeed(MS_TO_CLOCKS(500), 30);
				need_brake = 0 ; 
		}
		else
			if (need_brake ==2)
			{
				//start_chspeed(MS_TO_CLOCKS(500), 80);
				need_brake = 0 ; 
			}
	}

}
