#ifndef __UTILS_H
#define __UTILS_H 1

extern uint32_t __vector_table[];

#define _VECTOR_RAM __vector_table

#define GPIO_PIN_MASK            0x1Fu
#define GPIO_PIN(x)              (((1)<<(x & GPIO_PIN_MASK)))

#define MS_TO_CLOCKS(x) 	x*50000
#define US_TO_CLOCKS(x)		x*50

#endif  


