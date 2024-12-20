#include <MKL25Z4.H>
#include <stdio.h>
#include <math.h>
#define IR_SENSOR_PIN 1
#define LED_RED_PIN   0

#define MASK(x) (1UL << (x))
void Delay(volatile unsigned int time_del);

void horizontal_servo(void);
void yon1 (void);



static int result;
static int b =0;


void Delay(volatile unsigned int time_del) {
    while (time_del--) {
        // Empty loop
    }
}



void horizontal_servo(void){
SIM->SOPT2 |= 0x01000000; /* use MCGFLLCLK as timer counter clock */

TPM0->SC = 0; /* disable timer */
TPM0->CONTROLS[1].CnSC = 0x20 | 0x08; 
TPM0->MOD = 26213;
TPM0->CONTROLS[1].CnV =b+560; 
TPM0->SC=0xC;




}
 
 void yon1 (void){
	 if(result<1025 && result>800){
b+=100;

}
else if(result<799 && result>424){
b+=0;

}
else if(result<425 && result>=0){
b+=-100;

}
 if (b>=2590){
b=2590;
}
else if (b<=0){
b=0;
}}
 
 
 
 
 
 
 
 
 
 
 
 
 
int main(void) {
	
	
	
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;

    // Configuring IR sensor pin as input
    PORTA->PCR[IR_SENSOR_PIN] = PORT_PCR_MUX(1);
    PTA->PDDR &= ~MASK(IR_SENSOR_PIN);

    // Configuring red LED pin as output
    PORTB->PCR[LED_RED_PIN] = PORT_PCR_MUX(1);
    PTB->PDDR |= MASK(LED_RED_PIN);

	
	
	
	SIM->SCGC5 |=(1UL<<12);
PORTD->PCR[1] =0x0400;
	SIM->SCGC6 |= 0x01000000; /* enable clock to TPM0 */
	SIM->SOPT2 |= 0x01000000; /* use MCGFLLCLK as timer counter clock */
	
	SIM->SCGC5 |= 0x2000; /* clock to PORTE */
PORTE->PCR[20] = 0; /* PTE20 analog input */
SIM->SCGC6 |= 0x8000000; /* clock to ADC0 */
ADC0->SC2 &= ~0x40; /* software trigger */

	ADC0->CFG1 = 0x40 | 0x10 | 0x08 |0x00;
	
	
  
 // Enable clock for Port d to configure PTD6
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;  

    // Configure PTA1 as GPIO with pull-up resistor enabled and interrupt on falling edge
    PORTD->PCR[6] = 0X103 ; // Pin MUX as GPIO
                       // Pull-up Enable
                       // Pull Select: Pull-up
	PTD->PDDR &= ~(1UL<<6);  
	
SIM_SCGC5 |= 0x400; /* enable clock to Port B */
PORTB_PCR9 = 0x100; /* make PTB19 pin as GPIO (See Table 2-4) */
PORTB_PCR8 = 0x100;
	PORTB_PCR10=0x100; 
PORTB_PCR11=0x100;
 PTB->PDDR |= (1UL<<9);/* make PTB19 as output pin */
PTB->PDDR |= (1UL<<8);
	PTB->PDDR |= (1UL<<10);
PTB->PDDR |= (1UL<<11);
	
    while (1) {
				horizontal_servo();

			ADC0->SC1[0] = 0; /* start conversion on channel 0 */
while(!(ADC0->SC1[0] & 0x80)) { } /* wait for conversion complete */
result=ADC0->R[0];
yon1();
        if ((PTD->PDIR &(1UL << 6)) == 0) {
					PTB->PSOR |= (1UL<<9);
					PTB->PCOR |= (1UL<<8);
				PTB->PSOR |= (1UL<<10);
					PTB->PCOR |= (1UL<<11);
				if ((PTA->PDIR & (1UL << IR_SENSOR_PIN)) != 0) {
            // IR sensor sends 1
            PTB->PCOR |= (1UL << LED_RED_PIN); // Turn on LED
        } else {
            // IR sensor sends 0
            PTB->PSOR |= (1UL << LED_RED_PIN); // Turn off LED
        }
}
				else {
					PTB->PCOR |=(1UL<<9);
					PTB->PCOR |= (1UL<<8);
					PTB->PCOR |=(1UL<<10);
					PTB->PCOR |= (1UL<<10);
					if ((PTA->PDIR & (1UL << IR_SENSOR_PIN)) != 0) {
            // IR sensor sends 1
            PTB->PCOR |= (1UL << LED_RED_PIN); // Turn on LED
        } else {
            // IR sensor sends 0
            PTB->PSOR |= (1UL << LED_RED_PIN); // Turn off LED
        }


				}
   Delay(50000);    }
    

	}