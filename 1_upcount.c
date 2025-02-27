#include <LPC17xx.h>
unsigned int i;
unsigned long LED;
int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON -> PINSEL0 &= 0xFF0000FF;
	LPC_GPIO0 -> FIODIR |= 0x00000FF0;
	while(1){
		for(LED = 0; LED < 256; LED++){
			LPC_GPIO0 -> FIOPIN = LED << 4;
			for(i = 0; i < 100000; i++);
		}
	}
}