/*
 * File:        lptmr.h
 * Purpose:     Provide common low power timer functions
 *
 * Notes:
 */

#ifndef __LPTMR_H__
#define __LPTMR_H__

#define LPTMR_USE_MCGIRCLK 0
#define LPTMR_USE_LPOCLK 1
#define LPTMR_USE_ERCLK32 2
#define LPTMR_USE_OSCERCLK 3
   
#define  LPTMR_irq_no           85 // Vector No 101

/********************************************************************/

/* Function prototypes */
void time_delay_ms(int count_val);
void lptmr_isr(void);
void LPTMR_init(int count, int clock_source);
void lptmr_init(int count, int clock_source);


/********************************************************************/

#endif /* __LPTMR_H__ */
