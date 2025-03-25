#include<LPC17xx.h>
unsigned char digits[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
unsigned int i;
int digi[4] = {4, 3, 2, 1};
int disp[4] = {0, 1, 2, 3};
void delay(void){
	for(i = 0; i < 1000; i++);
}
void displayDigit(int digit, int pos){
	LPC_GPIO0->FIOPIN = digits[digit] <<4;
	LPC_GPIO1->FIOPIN = disp[pos] << 23;
	delay();
}
int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0 &= 0xF0000FFF;
	LPC_PINCON->PINSEL3 &= 0xF07FFFFF;
	LPC_GPIO0->FIODIR |= 0xFF << 4;
	LPC_GPIO1->FIODIR |= (15 << 23);
	while(1){
		displayDigit(digi[0], 0);
		displayDigit(digi[1], 1);
		displayDigit(digi[2], 2);
		displayDigit(digi[3], 3);
	}
}
