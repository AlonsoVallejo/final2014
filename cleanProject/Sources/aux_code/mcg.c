/*
 * K60 Learning Modules
 * Rochester Institute of Technology
 * Department of Computer Engineering
 *
 * Authors: Dan Cheung, Josh Watts, Sam Skalicky
 *
 * Title: Lab 3 - Multipurpose Clock Generator
 *
 * Description: This file configures the MCG to run in
 * 			Bypassed Low Power External (BLPE) mode (50 MHz)
 *
 * Date: 9/1/2011
 *
 * Revision: 1.0
 *
 * Filename: mcg.c
 */
#include "derivative.h"
#include "mcg.h"

/*
 *	This method configures the Multipurpose Clock Generator (MCG) to run in
 *	Bypassed Low Power External (BLPE) mode from FEI mode. On the TWR-K60N512
 *	Bypassed board, this would be 50 MHz.
 *
 */
void MCG_FEI_BLPE() {
	// Select External Reference Clock in MCG (Transition to FBE from FEI)
	// C1[CLKS] written to 10
	MCG_C1 = MCG_C1_CLKS(2);

	// Wait for Reference clock to switch to external reference
	while (MCG_S & MCG_S_IREFST_MASK);

	// Wait for MCGOUT to switch over to the external reference clock
	while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2);

	// Transition to bypassed low power external mode (BLPE)
	// C2[LP] written to 1
	MCG_C2 = MCG_C2_LP_MASK;
}

/*
 *	This method configures the Multipurpose Clock Generator (MCG) to run in
 *	FLL Engaged Internal (FEI) mode from BLPE mode. For K60 0M33Z masks, this
 *	is 25 MHz, otherwise it is 20.97 MHz.
 *
 */
void MCG_BLPE_FEI() {
	// Transition to FBE
	MCG_C2 = 0;

	// Transition to FEI
	MCG_C1 = MCG_C1_IREFS_MASK;

	// Wait for Reference clock to switch to internal reference
	while ((MCG_S & MCG_S_IREFST_MASK) == 0);

	// Wait for MCGOUT to switch over to the FLL clock
	while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x0);
}
