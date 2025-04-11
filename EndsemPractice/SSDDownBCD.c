//Display a 4-digit BCD down counter from 9999 to 0000 on 4 multiplexed 7-segment displays.
//P0.4 to P0.11 as data lines 
//P1.23 to P1.26 as digit select lines
#include<lpc17xx.h>
#define BIT(n, x) (n << x)
unsigned char seg_code[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
int digits[4] = {9, 9, 9, 9};

void delay(){
    int i;
    for(i = 0; i < 10000; i++);
}
//display all 4 digits by multiplexing
void display(){
    unsigned int i;
    for (i = 0; i < 4; i++){
        LPC_GPIO1->FIOPIN = BIT(1, (23+i));                 //enable digit i
        LPC_GPIO0->FIOPIN = BIT(seg_code[digits[i]], 4);    //send digit to display
        delay();
    }
    LPC_GPIO0->FIOCLR = BIT(0xFF, 4);                       //clear all segments
}

int main(){
    SystemInit();
    SystemCoreClockUpdate();

    LPC_GPIO0->FIODIR |= BIT(0xFF, 4);                      //P0.4 to P0.11 as o/p
    LPC_GPIO1->FIODIR |= BIT(0xF, 23);                      //P1.23 to P1.26 as o/p

    while(1){
        for(digits[3] = 9; digits[3] >= 0; digits[3]--)
            for(digits[2] = 9; digits[2] >= 0; digits[2]--)
                for(digits[1] = 9; digits[1] >= 0; digits[1]--)
                    for(digits[0] = 9; digits[0] >= 0; digits[0]--)
                        display();
    }
}