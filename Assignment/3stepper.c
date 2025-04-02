#include<lpc17xx.h>
unsigned long int step;
unsigned int i = 0, j = 0;
void initTimer(){
    LPC_TIM0->CTCR = 0x0;   //timer mode
    LPC_TIM0->TCR = 0x02;   //reset timer
    LPC_TIM0->PR = 2499;    //for given Pclk and Tres
    LPC_TIM0->MR0 = 9;      //10 * 100 micro seconds = 1 ms
    LPC_TIM0->MCR = 0x02;   //reset on MR0 match
    LPC_TIM0->EMR = 0x20;   //toggle output on MR0 match
    LPC_TIM0->TCR = 0x01;   //start timer
}
void delay(){
    initTimer();
    while(!(LPC_TIM0->EMR & 1));
}
void clockwise(){
    step = (1 << 23);  //start at P3.23
    for(i = 0; i < 4; i++){
        LPC_GPIO3->FIOPIN = step;
        delay(); 
        step <<= 1;
    }
}
void anticlockwise(){
    step = (1 << 26);   //start at P3.26
    for(i = 0; i < 4; i++){
        LPC_GPIO3->FIOPIN = step;
        delay();
        step >>= 1;
    }
}
int main(){
    SystemInit();
    SystemCoreClockUpdate();
    LPC_GPIO3->FIODIR |= 0x0F << 23;
    while(1){
        for(j = 0; j < 50; j++)
            clockwise();
        delay();
        for(j = 0; j < 50; j++)
            anticlockwise();
        delay();
    }
}