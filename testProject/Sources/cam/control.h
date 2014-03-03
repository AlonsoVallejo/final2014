/*
 * sensors.h
 *
 *  Created on: Jan 19, 2013
 *      Author: nexz
 */

#ifndef SENSORS_H_
#define SENSORS_H_

// USE LOGGING?
#define LOG_FRAME_DATA

void do_brake_to_0();//added by me!!!!

// LINE DETECTION PARAMETERS ----------------------

#define OFFSET 5 			//  muchie unde abs(frame[i] - frame[i-OFFSET]) > prag 
#define OFFSET_CORRECTION 2  // compenseaza eroarea pozitiei liniei datorata OFFSET
#define MARGIN  3			// ofera posibilitatea ignorarii marginii frame-ului
#define MIN_START_GAP 3		// lungime minima in pixeli a zonei albe la start
#define MAX_START_GAP 20	 // lungime maxima in pixeli a zonei albe la start
#define DEFAULT_LINE_THRESHOLD 15 // diferenta minima de intensitate pt detectie edge 
#define LINE_MIN_WIDTH 4     
#define LINE_MAX_WIDTH 25

//define 2014
//for correcting the wrong center
#define CENTER_CORRECTION 6

//curve offset depending on curve direction
#define CURVE_OFFSET 4

#define BRAKE_THRESHHOLD 15
#define LINE_BUF_MAX 64

#define THRESHOLD_ACC 25
#define THRESHOLD_BRAKE 60
#define DIFFERENCE 2
#define SAMPLE_SIZE 15

// FRAME INFO AND DATA STRUCTS ---------------------

// to be used in ringbuff; TO DO - include time and speed
typedef struct frame_info_t {
	int linepos;
	int type; 
} frame_info;

// to be used for whole frame log
typedef struct frame_data_t {
	unsigned int time;
	frame_info info;
	unsigned char data[128];
} frame_data;


// to be used for current log
typedef struct current_read_t 
{
	const unsigned char padding;
	short left;
	short right;
	unsigned char pwm;
	unsigned int time;
	float W_left;
	float W_right;
	float PWM_L;
	float PWM_R;
} current_read;

typedef struct log_entry_t 
{
	short pwm;
	short right;
//	float W_right;
//	float PWM_R;
} log_entry;


// RING BUFFER SETUP ------------------------------ 
// temporary unused
#define RBUFF_TYPE frame_info
#define RBUFF_SIZE 50
#include "ringbuff.h"


// LOGGING SETUP ---------------------------------

#ifdef LOG_FRAME_DATA
//#define LOG_ENTRY_TYPE frame_data // change for logging other types
#define LOG_ENTRY_TYPE log_entry // change for logging other types
#define LOG_SIZE 1500
//#include "log.h" DECOMENTEAZA ??
#endif



// FRAME TYPES ------------------------------------
#define FRAME_NONE 0
#define FRAME_ERROR 1
#define FRAME_START 2
#define FRAME_LINE 3


// MOTOR FEEDBACK
// valorile scrise in reg SC1 pentru inceperea conversiei feedback-ului de la motoare
#define ADC0_SC1A_VAL_LEFT (ADC_SC1_ADCH(0)|ADC_SC1_DIFF_MASK|ADC_SC1_AIEN_MASK)
#define ADC0_SC1A_VAL_RIGHT (ADC_SC1_ADCH(1)|ADC_SC1_DIFF_MASK|ADC_SC1_AIEN_MASK)


// DECLARATIONS FOR OTHER SOURCES -----------------


//average function for last 20 vals of line pos
int avg();

// called in cam.c in cam_pit0_isr() before starting to clock out pixels
void got_frame();
// referenced in init.c in init_adc()
void motors_adc0_isr();


// SHARED VARIABLES
#ifndef IN_CONTROL_C
extern frame_info crnt_frame; // needed? in main.c
extern frame_data framebuff; // needed in cam.c
extern unsigned int time_5ms; // needed mostly everywhere
extern unsigned int last_error_time; // needed in main.c 
extern unsigned int last_start_time; // needed in main.c
extern unsigned int last_line_time;  // timpul ultimului start (0 daca a fost procesat)
extern unsigned int prev_start_time; // timpul penultimului start 
extern unsigned char current_reference; // for later use with PID
extern int logging; 			// logging activ ? 
#else 
frame_info crnt_frame;
frame_data framebuff;
unsigned int time_5ms = 1;
unsigned int last_error_time = 0;
unsigned int last_start_time = 0;
unsigned int last_line_time = 0;
unsigned int prev_start_time = 0;
unsigned char current_reference;
int logging = 0;
#endif


#endif /* SENSORS_H_ */
