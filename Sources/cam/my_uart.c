/*
 * my_uart.c
 *
 *  Created on: Jan 15, 2014
 *      Author: John
 */
#include "my_uart.h"
#include "common.h"
#include "uart.h"
//sursele mele facute de mine
//PORT_PCR_MUX(3) enables UART
void init_uart(int uart_channel) 
{
	if (uart_channel==0)
	{
		//activation code for UART channel 0 
		SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
		PORTB_PCR16 = PORT_PCR_MUX(3);
		PORTB_PCR17 = PORT_PCR_MUX(3);
		PORTE_PCR8 = PORT_PCR_MUX(0);
		PORTE_PCR9 = PORT_PCR_MUX(0);
	}
	else
	{
		//activation code for UART channel 5 through the USB OSJTAG 
		SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
		PORTE_PCR8 = PORT_PCR_MUX(3);
		PORTE_PCR9 = PORT_PCR_MUX(3);
	}
	
}
