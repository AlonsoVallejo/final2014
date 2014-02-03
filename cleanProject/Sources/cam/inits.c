#include "derivative.h"
#include "utils.h"
#include "cam.h"
#include "uart.h"

void init_uart()
{
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB_PCR16 = PORT_PCR_MUX(3);
	PORTB_PCR17 = PORT_PCR_MUX(3);
	uart_init(TERM_PORT,_50MHZ_CLOCK,DEFAULT_BAUD); //UART output: usb debug port
													//BAUD: 115200
													//OSC: 50 MHZ
}

void init_gpio() {
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;         // enable clock distribution
	PORTC_PCR0 = PORT_PCR_MUX(1);		  // set pin0 on PORTC muxing 	
	PORTC_PCR1 = PORT_PCR_MUX(1);		  // set pin0 on PORTC muxing	
	GPIOC_PDDR = GPIO_PDDR_PDD(GPIO_PIN(SI_PIN) | GPIO_PIN(CLK_PIN) ); // set pins for output
	GPIOC_PCOR = GPIO_PIN(1)|GPIO_PIN(0); // clear pin values 
}

uint8 calibrate_adc1() {
	unsigned short cal_var;

	ADC1_SC2 &= ~ADC_SC2_ADTRG_MASK; // Enable Software Conversion Trigger for Calibration Process    - ADC0_SC2 = ADC0_SC2 | ADC_SC2_ADTRGW(0);   
	ADC1_SC3 &= (~ADC_SC3_ADCO_MASK & ~ADC_SC3_AVGS_MASK); // set single conversion, clear avgs bitfield for next writing
	ADC1_SC3 |= (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3)); // Turn averaging ON and set at max value ( 32 )


	ADC1_SC3 |= ADC_SC3_CAL_MASK; // Start CAL
	while (!(ADC1_SC1A & ADC_SC1_COCO_MASK))
		; // Wait calibration end

	if ((ADC1_SC3 & ADC_SC3_CALF_MASK)) {
		return (1); // Check for Calibration fail error and return 
	}
	// Calculate plus-side calibration
	cal_var = 0x00;

	cal_var = ADC1_CLP0;
	cal_var += ADC1_CLP1;
	cal_var += ADC1_CLP2;
	cal_var += ADC1_CLP3;
	cal_var += ADC1_CLP4;
	cal_var += ADC1_CLPS;

	cal_var = cal_var / 2;
	cal_var |= 0x8000; // Set MSB

	ADC1_PG = ADC_PG_PG(cal_var);

	// Calculate minus-side calibration
	cal_var = 0x00;

	cal_var = ADC1_CLM0;
	cal_var += ADC1_CLM1;
	cal_var += ADC1_CLM2;
	cal_var += ADC1_CLM3;
	cal_var += ADC1_CLM4;
	cal_var += ADC1_CLMS;

	cal_var = cal_var / 2;

	cal_var |= 0x8000; // Set MSB

	ADC1_MG = ADC_MG_MG(cal_var);

	ADC1_SC3 &= ~(ADC_SC3_CAL_MASK); /* Clear CAL bit */

	return (0);
}

void adc1_isr();

void init_adc() {
	//adc1_dm0 - twrpi adc pin;  adc1_dm1 - potentiometer
	SIM_SCGC3 |= (SIM_SCGC3_ADC1_MASK);
	if (calibrate_adc1())
		printf("Calibration error\n");
	
	//set adc conversion complete isr
	disable_irq(58);
	//__vector_table[74] = (uint32)&adc1_isr;
	enable_irq(58);	
	
	ADC1_CFG1 = 0; // 8 bit conversion
	ADC1_CFG2 = 0; // default
	ADC1_SC1A = 0x1f; // 0x1F -disable  20 - potentiometer ; single; int coco
	ADC1_SC2 = 0;
	ADC1_SC3 &= 0xF; //disable functs like continuous conversion
	ADC1_PGA = 0; //not amplifying 

}

