#include "derivative.h"
#include "utils.h"
#include "cam.h"
#include "pwm.h"
#include "testing.h"

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
	//init_adc();
	//other initializations used in this experiment
	//init_cam();
	init_pwm();
	
	
	out_char('s');
	while (1) {
		while(char_present()==0);
		c = in_char();
		//out_char(c);
		test(c);
	}//While(1)
}//Main