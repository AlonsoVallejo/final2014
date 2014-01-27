/*
 * gpio.h
 *
 *  Created on: 14.01.2014
 *      Author: Iulian
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "derivative.h"

/*		defines		*/

/*SECTION signal actions*/
#define SELECTION_MASK		(1 << 26)
#define SELECTION_OFF		GPIOE_PDOR &= ~(SELECTION_MASK);
#define SELECTION_ON		GPIOE_PDOR |= SELECTION_MASK;

/*ENDL signal actions*/
#define ENDL_MASK		(1 << 14)
#define ENDL_OFF		GPIOD_PDOR &= ~(ENDL_MASK);
#define ENDL_ON			GPIOD_PDOR |= ENDL_MASK;

/*ENDR signal actions*/
#define ENDR_MASK		(1 << 3)
#define ENDR_OFF		GPIOB_PDOR &= ~(ENDR_MASK);
#define ENDR_ON			GPIOB_PDOR |= ENDR_MASK;


#endif /* GPIO_H_ */
