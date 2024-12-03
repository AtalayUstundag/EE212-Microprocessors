#include <MKL25Z4.H>
#include <stdio.h>
#define frequency_of_PWM 50        // servo dönüsü için PVM frekansi 50 hz
#define presec 16            // 16 ile bölünecek
#define frequency_of_TPM 21600000  // TPM'nin frekansi (Hz birim)
#define MASK(x) (1UL << (x))

void PWM_kodu(void);
void Servo_donus_ayarla(int Width_of_pulse);
void Delay(volatile unsigned int time_del);
void butonun_interruptini_ayarla(void);
void PORTD_IRQHandler(void);

volatile int way_of_turning = 0; // 0 fiçin ilk durum yani saga dön, 1 için 2. durum yani sola dön


void Delay(volatile unsigned int time_del) {
    while (time_del--) {
        
    }
}
void PWM_kodu(void) {
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; // port e nin clokunu aç
    PORTE->PCR[20] = PORT_PCR_MUX(3); // PWM sinyalini E20den gönder 

    SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK; //
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // TPMx 1. Mod
    TPM1->SC = 0;
    TPM1->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;
    TPM1->MOD = (frequency_of_TPM / (presec * frequency_of_PWM)) - 1; //  MOD ayarlandi 

    TPM1->CONTROLS[0].CnV = 750; // Start at 0 degrees
    TPM1->SC = TPM_SC_CMOD(1) | TPM_SC_PS(4);
}

void Servo_donus_ayarla(int Width_of_pulse) {
    TPM1->CONTROLS[0].CnV = Width_of_pulse;
}

int main(void) {
    PWM_kodu();               // pwm kodunu aktif et
    butonun_interruptini_ayarla(); // Butonun interruptini çagir 

    while (1) {
        if (way_of_turning == 0) {
            // ilk durum yani saga git
            Servo_donus_ayarla(659);  Delay(950409); // 0 derece 
						if (way_of_turning == 1) {
								continue;
						}
            Servo_donus_ayarla(1078); Delay(950409); // 30 derece
						if (way_of_turning == 1) {
								continue;
						}
            Servo_donus_ayarla(1530); Delay(950409); // 60 derece
						if (way_of_turning == 1) {
								continue;
						}
            Servo_donus_ayarla(2000); Delay(950409); // 90 derece
						if (way_of_turning == 1) {
								continue;
						}
            Servo_donus_ayarla(1530); Delay(950409); // 60 derece
						if (way_of_turning == 1) {
								continue;
						}
            Servo_donus_ayarla(1078); Delay(950409); // 30 derece
            if (way_of_turning == 1) {
								continue;
						}
        } else {
            // ikinci durum yani way_of_turning = 1,sola dön
            Servo_donus_ayarla(3270);  Delay(950409); // 180 derece
					if (way_of_turning == 0) {
								continue;
						}
            Servo_donus_ayarla(2840);  Delay(950409); // 150 derece
						if (way_of_turning == 0) {
								continue;
						}
            Servo_donus_ayarla(2400);  Delay(950409); // 120 derece
						if (way_of_turning == 0) {
								continue;
						}
            Servo_donus_ayarla(1971);  Delay(950409); // 90 derece 
						if (way_of_turning == 0) {
								continue;
						}
            Servo_donus_ayarla(2400);  Delay(950409); // 120 derece
						if (way_of_turning == 0) {
								continue;
						}
            Servo_donus_ayarla(2840);  Delay(950409);// 150 dereece
						if (way_of_turning == 0) {
								continue;
						}
        }
    }

    return 0;
}



void butonun_interruptini_ayarla(void) {
    // port dnin clockunu aç
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;  

    //  PTD6 YI GPIO OLARAK AYARLA pull-up resistor YAP VE interrupt on falling edge
    PORTD->PCR[6] = PORT_PCR_MUX(1) |   
                    PORT_PCR_PE_MASK |   // Pull-up Enable
                    PORT_PCR_PS_MASK |   // Pull Select: Pull-up
                    PORT_PCR_IRQC(0xA);  // iNTERRUPTI FALLING EDGE YAP (1010)

    // PORT D INTERRUPTINI AÇ
    NVIC_EnableIRQ(PORTD_IRQn);         
}

void PORTD_IRQHandler(void) {
    // Clear the interrupt flag by writing a logic 1 to the ISFR register
    PORTD->ISFR = 0xffffffff;  // Clear interrupT
    // DÖNÜS YONUNU HER DEFAASINDA DEGISTIR
    way_of_turning = !way_of_turning;
}