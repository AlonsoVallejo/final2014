/*
 * gpio.h
 *
 *  Created on: Feb 27, 2014
 *      Author: admin
 */

#ifndef GPIO_H_
#define GPIO_H_

#define LED1_MASK (1<<10)
#define LED2_MASK (1<<11)
#define LED3_MASK (1<<28)
#define LED4_MASK (1<<29)

#define LED1_TOGGLE GPIOA_PTOR |= LED1_MASK
#define LED1_ON GPIOA_PSOR |= LED1_MASK
#define LED1_OFF GPIOA_PCOR |= LED1_MASK

#define LED2_TOGGLE GPIOA_PTOR |= LED2_MASK
#define LED2_ON GPIOA_PSOR |= LED2_MASK
#define LED2_OFF GPIOA_PCOR |= LED2_MASK

#define CONF(i) {switch(i){case 0 : LED3_ON;LED4_ON;break; case 1:LED3_OFF;LED4_OFF;break; case 2 : LED3_ON;LED4_OFF;break; }} //TO DO add forth mode



#define LED3_TOGGLE GPIOA_PTOR |= LED3_MASK
#define LED3_ON GPIOA_PSOR |= LED3_MASK
#define LED3_OFF GPIOA_PCOR |= LED3_MASK

#define LED4_TOGGLE GPIOA_PTOR |= LED4_MASK
#define LED4_ON GPIOA_PSOR |= LED4_MASK
#define LED4_OFF GPIOA_PCOR |= LED4_MASK

#endif /* GPIO_H_ */
