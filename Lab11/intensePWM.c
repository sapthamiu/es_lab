//WAP to vary the intensity of an LED using PWM
#include<LPC17xx.h>
void initPWM(){
    LPC_PINCON->PINSEL3 |= 0x8000;  //PWM1.4 o/p for P1.23 func 2
    LPC_PWM1->PCR = 0x1000;         //enable PWM1.4
    LPC_PWM1->PR = 0;
    LPC_PWM1->MR0 = 30000;          //period = 10ms if Pclk = Cclk/4
    LPC_PWM1->MCR = (1 << 1);       //reset PWM TC on MR0 match
    LPC_PWM1->LER = 0xFF;           //update values in MR0 and MR1
    LPC_PWM1->TCR = 0x2;            //reset counter TC and PC
    LPC_PWM1->TCR = 0x9;            //enable TC and PC
}

void updatePW(unsigned int pw){
    LPC_PWM1->MR4 = pw;             //update MR4 with new value
    LPC_PWM1->LER = 0xFF;           //load the MR4 new value at start of next cycle
}

void delayms(unsigned int ms){
    LPC_TIM0->CTCR = 0x0;           //timer mode
    LPC_TIM0->PR = 2;               //increment TC at every 2 Pclk
    LPC_TIM0->TCR = 0x02;           //reset timer
    LPC_TIM0->TCR = 0x01;           //enable timer
    while(LPC_TIM0->TC < ms);
    LPC_TIM0->TCR = 0x0;            //disable timer
}

int main(){
    int pws[] = {0, 3000, 6000, 9000, 12000, 15000, 18000, 21000, 24000, 27000};
    const int n = 10;
    int count = 1;
    int dir = 0;                    //0->increasing, 1->decreasing
    initPWM();

    while(1){
        updatePW(pws[count]);
        delayms(10000);
        if(count == (n-1) || count == 0)
            dir = !dir;
        if(dir) count--;
        else count ++;
    }
}