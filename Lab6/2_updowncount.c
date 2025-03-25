#include <LPC17xx.h>
unsigned int i;
int LED;
int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON -> PINSEL0 &= 0xFF0000FF;
	LPC_PINCON -> PINSEL1 &= 0xFFFFF3FF;
	LPC_GPIO0 -> FIODIR |= 0x00000FF0;
	LPC_GPIO0 -> FIODIR &= 0xFFDFFFFF;
	while(1){
		if( LPC_GPIO0 -> FIOPIN & 1 << 21){
			for(LED = 0; LED < 256; LED++){
				LPC_GPIO0 -> FIOPIN = LED << 4;
				for(i = 0; i < 100000; i++);
				if(!( LPC_GPIO0 -> FIOPIN & 1 << 21))
						break;
			}
		}
		else{
			for(LED = 255; LED >= 0; LED--){
				LPC_GPIO0 -> FIOPIN = LED << 4;
				for(i = 0; i < 100000; i++);
				if( LPC_GPIO0 -> FIOPIN & 1 << 21)
						break;
			}
		}
	}
}