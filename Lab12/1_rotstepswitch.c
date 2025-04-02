#include<lpc17xx.h>
unsigned long int step, var2;
unsigned int i = 0, j = 0, k = 0;
void clockwise(void){
	step = 0x00000008;
	for(i = 0; i <= 3; i++){
		step <<= 1;
		LPC_GPIO0->FIOPIN = step;
		for(k = 0; k < 3000; k++);
	}
}
void anticlockwise(void){
	step = 0x00000100;
	for(i = 0; i <= 3; i++){
		step >>= 1;
		LPC_GPIO0->FIOPIN = step;
		for(k = 0; k < 3000; k++);
	}
}
int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0 = 0xFFFF00FF;
	LPC_GPIO0->FIODIR = 0x000000F0;
	LPC_PINCON->PINSEL4 &= ~(3 << 24);  //P2.12 as GPIO
  LPC_GPIO2->FIODIR &= ~(1 << 12);    //P2.12 as input
	while(1){
		if(!(LPC_GPIO2->FIOPIN & (1 << 12))){
			for(j = 0; j < 50; j++)
				clockwise();
			for(k = 0; k < 65000; k++);
		}
		else{
			for(j = 0; j < 50; j++)
				anticlockwise();
			for(k = 0; k < 65000; k++);
		}
	}
}