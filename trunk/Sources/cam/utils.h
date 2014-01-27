#ifndef __UTILS_H
#define __UTILS_H 1

extern uint32 __VECTOR_RAM[];

#define GPIO_PIN_MASK            0x1Fu
#define GPIO_PIN(x)              (((1)<<(x & GPIO_PIN_MASK)))

#define MS_TO_CLOCKS(x) 	x*50000
#define US_TO_CLOCKS(x)		x*50

void check_cmds(); //utils.c
void init_gpio(); //inits.c
void init_uart(); // inits.c
void cam_adc1_isr(); // cam.c
void motors_adc0_isr(); // nwmotors.c

#endif  


