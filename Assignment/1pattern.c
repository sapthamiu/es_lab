#include<lpc17xx.h>
unsigned char pattern[8] = {
    0xFF, //1111 1111
    0x7E, //0111 1110
    0x3C, //0011 1100
    0x18, //0001 1000
    0x18,
    0x3C,
    0x7E,
    0xFF
};
void delay(int ms){
	int i;
    for(i = 0; i < ms * 5000; i++);
}
int main(void){
		int i;
    LPC_GPIO0->FIODIR |= 0x0FF0;
    LPC_GPIO1->FIODIR &= ~(1 << 26);
    while(1){
        if(!(LPC_GPIO1->FIOPIN & (1 << 26))){
            for(i = 0; i < 8; i++){
                LPC_GPIO0->FIOCLR = 0x0FF0;
                LPC_GPIO0->FIOSET = (pattern[i] << 4);
                delay(500);
            }
        }
        else 
            LPC_GPIO0->FIOCLR = 0x0FF0;
    }
}