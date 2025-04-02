#include<lpc17xx.h>
unsigned long int var1, var2;
unsigned int i = 0, j = 0, k = 0;
void clockwise(void){
	var1 = 0x00000008;
	for(i = 0; i <= 3; i++){
		var1 <<= 1;
		LPC_GPIO0->FIOPIN = var1;
		for(k = 0; k < 3000; k++);
	}
}
void anticlockwise(void){
	var1 = 0x00000100;
	for(i = 0; i <= 3; i++){
		var1 >>= 1;
		LPC_GPIO0->FIOPIN = var1;
		for(k = 0; k < 3000; k++);
	}
}
int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0 = 0xFFFF00FF;
	LPC_GPIO0->FIODIR = 0x000000F0;
	while(1){
		for(j = 0; j < 50; j++)
			clockwise();
		for(k = 0; k < 65000; k++);
		for(j = 0; j < 50; j++)
			anticlockwise();
		for(k = 0; k < 65000; k++);
	}
}