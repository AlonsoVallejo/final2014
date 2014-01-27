/*
 * File:        common.h
 * Purpose:     File to be included by all project files
 *
 * Notes:
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/********************************************************************/

/*
 * Debug prints ON (#define) or OFF (#undef)
 */
#define DEBUG
#define DEBUG_PRINT

/* 
 * Include the generic CPU header file 
 */
#include "arm_cm4.h"

/* 
 * Include the platform specific header file 
 */
#if (defined(TWR_K70F120M))
  #include "twr-k70f120m.h"
#elif (defined(TWR_K60F120M))
  #include "twr-k60f120m.h"
#elif (defined(FIREBIRD))
  #include "firebird.h"
#else
  #error "No valid platform defined"
#endif

/* 
 * Include the cpu specific header file 
 */
#if (defined(CPU_MK70F120))
  #include "MK70F15.h"
#elif (defined(CPU_MK60F120))
  #include "MK60F12.h"
#else
  #error "No valid CPU defined"
#endif

/* 
 * Include any toolchain specfic header files 
 */
#if (defined(CW))
  #include "cw.h"
#elif (defined(IAR))
  #include "iar.h"
#else
#warning "No toolchain specific header included"
#endif

/* 
 * Include common utilities
 */
#include "assert.h"
#include "io.h"
#include "startup.h"
#include "stdlib.h"

#if (defined(IAR))
	#include "intrinsics.h"
#endif

/********************************************************************/

#endif /* _COMMON_H_ */
