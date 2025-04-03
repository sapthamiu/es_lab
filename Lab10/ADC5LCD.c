//WAP to configure and read analog data from ADC channel no. 5 and display the digital data on LCD
#include<LPC17xx.h>
#include<stdio.h>
#include "LCDmessage.c"
#define Vref 3.3
#define full_scale 0xFFF    //12 bit ADC
int main(){
    unsigned long adcVal;
    unsigned int i;
    float Vin;
    char v[20], dval[20];
    unsigned char msg3[] = "Analog I/p: ";
    unsigned char msg4[] = "ADC o/p: ";
    SystemInit();
    SystemCoreClockUpdate();
    LPC_SC->PCONP |= (1 << 15); //power for GPIO block
    lcd_init();
    LPC_PINCON->PINSEL3 |= 0xc0000000;  //P1.31 as AD0.5
    LPC_SC->PCONP |= (1 << 12);         //enable the peripheral ADC
    SystemCoreClockUpdate();

    lcd_comdata(0x80, 0);
    delay_lcd(800);
    lcd_puts(msg3);
    lcd_comdata(0xC0, 0);
    delay_lcd(800);
    lcd_puts(msg4);

    while(1){
        LPC_ADC->ADCR = (1 << 5) | (1 << 21) | (1 << 24);   //ADC0.5, start conversion
        while(!(LPC_ADC->ADDR5 & 0x80000000));  //wait till done bit is 1 (conversion complete)
        adcVal = LPC_ADC->ADDR5;
        adcVal >>= 4; //get the 12 bit result
        Vin = (((float) adcVal * Vref)) / (float) full_scale; //calculate voltage
        sprintf(v, "%.2f V", Vin);
        sprintf(dval, "%x", adcVal);
        for(i = 0; i < 2000; i++);
        lcd_comdata(0x89, 0);
        delay_lcd(800);
        lcd_puts(v);

        lcd_comdata(0xC8, 0);
        delay_lcd(800);
        lcd_puts(dval);
        for(i = 0; i < 200000; i++);
        for(i = 0; i < 7; i++)
            v[i] = dval[i] = 0x00;
        adcVal = 0;
        Vin = 0;
    }
}