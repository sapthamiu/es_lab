#include <LPC17xx.h>
unsigned int i;
unsigned long LED = 0x00000010;
int place;
int main(void){
	LPC_PINCON -> PINSEL0 &= 0xFF0000FF;
	LPC_GPIO0 -> FIODIR |= 0x00000FF0;
	LPC_PINCON -> PINSEL1 &= 0xFFFFF3FF;
	LPC_GPIO0 -> FIODIR &= 0xFFDFFFFF;
	while(1){
		if(LPC_GPIO0 ->FIOPIN & 1<<21){
			for(place = 8; place >= 0; place--){
				LPC_GPIO0 -> FIOPIN = LED<<place;
				for(i = 0; i < 100000; i++);
			}
		}
	}
}