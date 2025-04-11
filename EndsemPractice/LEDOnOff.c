//Blink LEDs serially ON and OFF using GPIO Port 0 (P0.4 to P0.11)
#include <lpc17xx.h>
#define BIT(n, x) (n << x)
void delay(){
    int i;
    for (i = 0; i < 10000; i++);
}
int main(){
    unsigned int i;

    SystemInit();
    SystemCoreClockUpdate();

    LPC_GPIO0->FIODIR |= BIT(0xFF, 4);    //P0.4 to P0.11 as o/p

    while(1){
        //turn on
        unsigned int LED = 0x10;        //P0.4
        for(i = 1; i <= 8; i++){
            LPC_GPIO0->FIOSET = LED;
            delay();
            LED <<= 1;
        }
        //turn off
        LED = 0x10;
        for(i = 1; i <= 8; i++){
            LPC_GPIO0->FIOCLR = LED;
            delay();
            LED <<= 1;
        }
    }
}