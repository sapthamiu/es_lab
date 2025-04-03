#include<LPC17xx.h>

#define PWM_PRD 30000

void initPWM(){
    LPC_PINCON->PINSEL3 |= 0x8000;  //PWM1.4 o/p for P1.23 func 2
    LPC_PWM1->PCR = 0x1000;         //enable PWM1.4
    LPC_PWM1->PR = 0;
    LPC_PWM1->MR0 = PWM_PRD;          //period = 10ms if Pclk = Cclk/4
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
void initKB(){
    LPC_GPIO2->FIODIR &= 0x0F;    //P0.0 to P0.3 as i/p (rows)
    LPC_GPIO1->FIODIR |= ~(0xF87FFFFF);    //P0.4 to P0.4 as o/p (cols)
}

void scan(){
    int col, row;
    //scan row 0 for key presses
    for(col = 0; col < 4; col++){
        LPC_GPIO->FIOSET = (1 << (col + 4));    //set column high
        for(row = 0; row < 4; row++)
            if(!(LPC_GPIO0->FIOPIN & (1 << row))){   //check if key is pressed
                LPC_GPIO0->FIOCLR = (1 << (col + 4));   //clear column
                return row + col * 4;           //return key number
            }
        LPC_GPIO0->FIOCLR = (1 << (col + 4));   //clear column
    }
    return -1;
}

int main(){
    initPWM();
    initKB();

    while(1){
        char key = scan();
        unsigned int pwmVal = 0;
        switch(key){
            case 0: pwmVal = PWM_PRD * 0.1;
                    break;
            case 1: pwmVal = PWM_PRD * 0.25;
                    break;
            case 2: pwmVal = PWM_PRD * 0.5;
                    break;
            case 3: pwmVal = PWM_PRD * 0.75;
                    break;
            default: pwmVal = 0;
                    break;
        }
        updatePW(pwmVal);
        delayms(10000);
    }
}