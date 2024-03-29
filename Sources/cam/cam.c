#include "cam.h"
#include "common.h"
#include "utils.h"
#include "debug.h" 

static int cam_state;
static int clk_count;
static int frame_count = 0;
extern int count_pit0;
uint8 camera_uart_buff[128];

//pit0 - si and first clk
//pit1 - clk

//added
void got_frame_uart() {
	int i;
	frame_count++;
	
	if(frame_count == 100 && DEBUG_CAM){  // send every 100th frame on uart if other debug types not active
		// begin frame sequence: [255 0 255 0]
		out_char(255);		 
		out_char(0);
		out_char(255);
		out_char(0);
		//
		for(i=0;i<NUM_CLOCKS/2;i+=1) {			
			out_char(camera_uart_buff[i]);
		}
		frame_count = 0;
	}
	
}

void adc1_isr()
{
	uint32 tmp;
	ADC1_SC1A &= ~ADC_SC1_AIEN_MASK; // disable adc interrupts for now
	tmp = ADC1_RA;					 // read conversion result
	if (clk_count<NUM_CLOCKS) 
		camera_uart_buff[clk_count/2] = (uint8)tmp; //put byte into buffer
	ADC1_SC1A |= ADC_SC1_AIEN_MASK;  // re-enable interrupts
}


void pit0_isr(void)
{
	PIT_TCTRL0 = 0;
	PIT_TCTRL1 = 0;

	switch (cam_state) {
	case 0: 
		GPIOC_PCOR = GPIO_PIN(CLK_PIN); // clear CLK 
		got_frame_uart();                    // process the last frame - send it
		GPIOC_PSOR = GPIO_PIN(SI_PIN);  // set SI
		PIT_LDVAL0 = QUARTER_CLK_TIMER; // set pit0 to a quarter of a CLK period  
		clk_count = 1;					// init CLK counter 
		cam_state = 1; 					// next state
		break;
	case 1:
		GPIOC_PSOR = GPIO_PIN(CLK_PIN); // set CLK
		cam_state = 2;					// next state
		break;
	case 2:
		GPIOC_PCOR = GPIO_PIN(SI_PIN);  // clear SI
		cam_state = 3;					// next state
		break;
	case 3:
		GPIOC_PCOR = GPIO_PIN(CLK_PIN); // clear CLK
		PIT_LDVAL0 = SI_TIMER;			//set pit0 to longer interval between SI's
		ADC1_SC1A = SC1A_VAL;			// trigger ADC conversion by write to SC1 
		PIT_TCTRL1 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; // enable pit1
		cam_state = 0;					// next state - first
		break;
	}

	PIT_TFLG0 = 1;						// clear interrupt flag for pit0
	PIT_TFLG1 = 1;						// clear interrupt flag for pit1
	PIT_TCTRL0 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //workaround to re-enable interrupts
}

void pit1_isr(void){
	if (clk_count%2==0) {
		GPIOC_PCOR = GPIO_PIN(CLK_PIN); // clear CLK
		ADC1_SC1A = SC1A_VAL;			// trigger ADC conversion by write to SC1 
	}
	else {
		GPIOC_PSOR = GPIO_PIN(CLK_PIN); // set CLK
	}
	clk_count+=1;
	PIT_TFLG1 = 1; 						// clear interrupt flag for pit1
	PIT_TCTRL1 = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK; //workaround to re-enable interrupts
}

void init_cam(){
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; 	//enable clock distribution to module
	PIT_MCR = 3;						//disable module clock
	init_adc();

	// set pit0 interrupt service routine  
	disable_irq(68);
	disable_irq(69);
	__VECTOR_RAM[84] = (uint32)&pit0_isr; 
	
	
	// set pit1 interrupt service routine
	
	__VECTOR_RAM[85] = (uint32)&pit1_isr;
	enable_irq(68);
	enable_irq(69);
	
	
	cam_state = 0;						// reset state
	PIT_LDVAL0 = QUARTER_CLK_TIMER;		// set pit0 to quarter of a CLK period initially
	PIT_LDVAL1 = HALF_CLK_TIMER;		// set pit1 to half CLK period   
	PIT_TCTRL0 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //enable pit0
	PIT_MCR = 1;						// enable module clock
}
