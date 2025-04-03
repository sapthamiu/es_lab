#include<LPC17xx.h>
#include<stdio.h>
#include "LCDmessage.c"
#define Vref 3.3
#define full_scale 0xFFF    //12 bit ADC
void initADC(){
    LPC_SC->PCONP |= (1 << 15);
    LPC_PINCON->PINSEL3 = (0x3 << 12) | (0x3 << 14); //P1.30 (AD0.4) and P1.31 (AD0.5) to ADC function
    LPC_ADC->ADCR = (1 << 4) | (1 << 5) | (1 << 21) | (1 << 16);   //enable C4 and C5, PDN, burst mode 
}

unsigned long readADC(unsigned char channel){
    LPC_ADC->ADCR |= (1 << channel);    //start conversion
    while(!(LPC_ADC->ADDR[channel] & 0x80000000));  //wait for conversion to complete
    return (LPC_ADC->ADDR[channel] >> 4) & 0xFFF;   //get the 12 bit result
}

void main(){
    unsigned long adcVal4, adcVal5, adcDiff;
    char diff[20];
    unsigned char msg[] = "ADC Diff: ";
    unsigned int i;
    SystemInit();
    SystemCoreClockUpdate();
    lcd_init();
    initADC();

    lcd_comdata(0x80, 0);   //set cursor to first line
    delay_lcd(800);
    lcd_puts(msg);

    while(1){
        //read ADC values
        adcVal4 = readADC(4);
        adcVal5 = readADC(5);

        adcDiff = adcVal5 - adcVal4;

        //convert to string
        sprintf(diff, "%lu", adcDiff);

        lcd_comdata(0xC0, 0);   //set cursor to 2nd line
        delay_lcd(800);
        lcd_puts(diff);
        for(i = 0; i < 200000; i++);
    }
}