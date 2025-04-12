#include<lpc17xx.h>
#define BIT(n, x) (n << x)

void initPWM(){
    LPC_PINCON->PINSEL3 |= BIT(1, 15);  //P1.23 (01) as PWM1.4 
    LPC_PWM1->PCR = BIT(1, 12);         //enable PWM1.4 o/p
    LPC_PWM1->PR = 0;                   //no prescaler
    LPC_PWM1->MR0 = 30000;              //PWM period = 10ms 
    LPC_PWM1->MR4 = 0;                  //start with 0% duty
    LPC_PWM1->MCR = BIT(1, 1);          //reset on MR0 match
    LPC_PWM1->LER = BIT(1, 0) | BIT(1, 4);//load MR0 and MR4
    LPC_PWM1->TCR = BIT(1, 1);          //reset TC and PC
    LPC_PWM1->TCR = BIT(1, 0) | BIT(1, 3);//enable TC and PC
}

void updatePW(unsigned int pw){
    LPC_PWM1->MR4 = pw;                 //update MR with new duty value
    LPC_PWM1->LER = BIT(1, 4);          //load MR4 value on next cycle
}

void delayms(unsigned int ms){
    LPC_TIM0->CTCR = 0x0;               //timer mode
    LPC_TIM0->PR = 2;                   //increment TC at every 3 cycles
    LPC_TIM0->TCR = 0x02;               //reset timer
    LPC_TIM0->TCR = 0x01;               //start timer
    while(LPC_TIM0->TC < ms);           //wait
    LPC_TIM0->TCR = 0x00;               //stop timer
}

int main(){
    int pws[] = {0, 3000, 6000, 9000, 12000, 15000, 18000, 21000, 24000, 27000};
    const int n = 10;
    int count = 1;
    int dir = 0;                        //0 = increasing brightness, 1 = decreasing
    
    initPWM();

    while(1){
        updatePW(pws[count]);
        delayms(500);
        if(count == (n-1) || count == 0)
            dir = !dir;
        if(dir) count--;
        else count++;
    }
}