/*
#include<LPC17xx.h>
unsigned char digits[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
unsigned int i;
int disp[4] = {0, 1, 2, 3};
int digi[4] = {9, 9, 9, 9};
void initTimer0(void){
	LPC_TIM0->CTCR = 0;
	LPC_TIM0->TCR = 0x02;
	LPC_TIM0->PR = 2;
	LPC_TIM0->MR0 = 999;
	LPC_TIM0->MCR = 2;
	LPC_TIM0->EMR = 0x20;
	LPC_TIM0->TCR = 1;
}
void delay(void){
	initTimer0();
	while(!(LPC_TIM0->TC >= LPC_TIM0->MR0));
}

void displayDigit(int digit, int pos){
	//LPC_GPIO0->FIOCLR = 0xFF << 4;
	//LPC_GPIO1->FIOCLR = (15 << 23);
	LPC_GPIO0->FIOPIN = digits[digit] << 4;
	LPC_GPIO1->FIOPIN = disp[pos] << 23;
	delay();
}
int main(){
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0 &= 0xF0000FFF;
	LPC_PINCON->PINSEL3 &= 0xF07FFFFF;
	LPC_GPIO0->FIODIR |= 0xFF << 4;
	LPC_GPIO1->FIODIR |= 15 << 23;
	while(1){
		for(digi[3] = 9; digi[3] >= 0; digi[3]--)
			for(digi[2] = 9; digi[2] >= 0; digi[2]--)
				for(digi[1] = 9; digi[1] >= 0; digi[1]--){
						for(digi[0] = 9; digi[0] >= 0; digi[0]--){
							displayDigit(digi[0], 0);
							displayDigit(digi[1], 1);
							displayDigit(digi[2], 2);
							displayDigit(digi[3], 3);
						}
						delay();
						LPC_GPIO0->FIOCLR |= 0xFF << 4;
				}
	}
}
*/
#include<lpc17xx.h>

unsigned char a[10]={0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
int arr[4]={9,9,9,9};
unsigned int i;

void initTimer0(void){
	LPC_TIM0->CTCR=0X0;
	LPC_TIM0->TCR=0X02;
	LPC_TIM0->PR=2;
	LPC_TIM0->MR0=999;
	LPC_TIM0->MCR=2;
	LPC_TIM0->EMR=0X20;
	LPC_TIM0->TCR=0X01;
}
void delay(void){
	initTimer0();
	while(!(LPC_TIM0->EMR&1));
}

int main(){
	SystemInit();
	SystemCoreClockUpdate();
	LPC_GPIO0->FIODIR |=0XFF0;
	LPC_GPIO1->FIODIR |=0XF<<23;
	
	while(1){
		for(arr[3]=9;arr[3]>=0;arr[3]--){
			for(arr[2]=9;arr[2]>=0;arr[2]--){
				for(arr[1]=9;arr[1]>=0;arr[1]--){
					for(arr[0]=9;arr[0]>=0;arr[0]--){
						for(i=0;i<4;i++){
							LPC_GPIO1->FIOPIN=i<<23;
							LPC_GPIO0->FIOPIN=a[arr[i]]<<4;
							delay();
						}
						LPC_GPIO0->FIOCLR |= 0X00000FF0;
					}
				}
			}
		}
	}
}
						