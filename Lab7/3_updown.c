#include<lpc17xx.h>

unsigned char a[10]={0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
int arr[4]={0, 0, 0, 0};
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
	LPC_GPIO0->FIODIR &= ~(1<<21);
	while(1){
		if(LPC_GPIO0->FIOPIN & (1<<21)){
			for(arr[3]=0;arr[3]<=9;arr[3]++)
				for(arr[2]=0;arr[2]<=9;arr[2]++)
					for(arr[1]=0;arr[1]<=9;arr[1]++)
						for(arr[0]=0;arr[0]<=9;arr[0]++){
							for(i=0;i<4;i++){
								LPC_GPIO1->FIOPIN=i<<23;
								LPC_GPIO0->FIOPIN=a[arr[i]]<<4;
								delay();
							}
							LPC_GPIO0->FIOCLR |= 0X00000FF0;
						}
		}
		else{
			for(arr[3]=9;arr[3]>=0;arr[3]--)
				for(arr[2]=9;arr[2]>=0;arr[2]--)
					for(arr[1]=9;arr[1]>=0;arr[1]--)
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