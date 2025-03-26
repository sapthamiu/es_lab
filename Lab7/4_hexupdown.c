#include <LPC17xx.h>

unsigned char tohex[16] = {0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71};
int arr[4] = {0, 0, 0, 0}; 

void initTimer(){
	LPC_TIM0->CTCR = 0x0;   
	LPC_TIM0->TCR  = 0x02; 
	LPC_TIM0->PR   = 2;     
	LPC_TIM0->MR0  = 999999; 
	LPC_TIM0->MCR  = 2;     
	LPC_TIM0->EMR  = 0x20;  
	LPC_TIM0->TCR  = 0x01;  
}

void delay(){
	initTimer();
	while (!(LPC_TIM0->EMR & 1)); 
}

void updateDisplay(){
	for (int i = 0; i < 4; i++) {
		LPC_GPIO1->FIOPIN = i << 23;      
		LPC_GPIO0->FIOPIN = tohex[arr[i]] << 4; 
		delay(); 
	}
}

void incrementCounter(){
	arr[0]++;
	for (int i = 0; i < 3; i++) {
		if (arr[i] > 15) {
			arr[i] = 0;
			arr[i + 1]++;
		}
	}
	if (arr[3] > 15) arr[3] = 0;
}

void decrementCounter(){
	if (arr[0] == 0) {
		arr[0] = 15;
		for (int i = 1; i < 4; i++) {
			if (arr[i] == 0) 
				arr[i] = 15;
			else {
				arr[i]--;
				break;
			}
		}
	} else 
		arr[0]--;
}

int main(){
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0 &= 0xFF0000FF; 
	LPC_GPIO0->FIODIR |= 0XFF0;   
	LPC_PINCON->PINSEL3 &= 0xF00FFFFF; 
	LPC_GPIO1->FIODIR |= 0xF << 23; 
	LPC_PINCON->PINSEL4 &= 0xFFCFFFFF; 
	LPC_GPIO2->FIODIR &= 0xFFFFEFFF;

	while (1){
		int switchState = LPC_GPIO2->FIOPIN & (1 << 12);
		if (switchState){
			incrementCounter();
		} else { 
			decrementCounter();
		}
		updateDisplay();
	}
}
