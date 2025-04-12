#include<lpc17xx.h>
#define BIT(n, x) (n << x)

void clockwise(){
    unsigned long int step = BIT(1, 4);         //P0.4
    unsigned int i, k;
    for(i = 1; i <= 4; i++){
        LPC_GPIO0->FIOPIN = step;
        for(k = 0; k < 3000; k++);
        step = BIT(step, 1);                    //move to next pin
    }
}

void anticlockwise(){
    unsigned long int step = BIT(1, 7);         //P0.7
    unsigned int i, k;
    for(i = 1; i <= 4; i++){
        LPC_GPIO0->FIOPIN = step;
        for(k = 0; k < 3000; k++);
        step >>= 1;                    //move to next pin
    }
}

int main(){
		unsigned int j, k;
    SystemInit();
    SystemCoreClockUpdate();
    LPC_GPIO0->FIODIR |= BIT(0xF, 4);
    while(1){
        for(j = 0; j < 50; j++)
            clockwise();
        for(k = 0; k < 65000; k++);
        for(j = 0; j < 50; j++)
            anticlockwise();
        for(k = 0; k < 65000; k++);
    }
}