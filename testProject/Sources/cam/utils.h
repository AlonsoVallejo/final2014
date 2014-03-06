#ifndef __UTILS_H
#define __UTILS_H 1

int io_printf (const char *fmt, ...);

extern uint32_t __vector_table[];



#define GPIO_PIN_MASK            0x1Fu
#define GPIO_PIN(x)              (((1)<<(x & GPIO_PIN_MASK)))

#define MS_TO_CLOCKS(x) 	x*50000
#define US_TO_CLOCKS(x)		x*50
#define TURATIE_TO_PWM(x)   x*1
#define VELOCITY_TO_BREAKING_PERIOD(x)	2+x/7

#define OPEN_REACTION 0
#define ACCELERATE 1
#define BRAKE_STAGE0 2
#define BRAKE_STAGE1 3
#define BRAKE_STAGE2 4

#define TURATIE_STANDARD 10
#define ACCH_INDEX 0
#define ACCL_INDEX 1
#define BRK_INDEX  2


#endif  
