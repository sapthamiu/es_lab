//WAP to configure and read analog data from ADC channel no. 5 and display the digital data on LCD
#include<LPC17xx.h>
#include<stdio.h>
#include "LCDmessage.c"
#define Vref 3.3
#define full_scale 0xFFF    //12 bit ADC

void ADCinit(){
    LPC_SC->PCONP |= (1 << 15); //power for GPIO block
    LPC_PINCON->PINSEL3 |= (0x3 << 14); //P1.31 as AD0.5
    LPC_SC->PCONP |= (1 << 12); //enable ADC
}
unsigned long readADC(unsigned char channel){
    LPC_ADC->ADCR = (1 << channel) | (1 << 21) | (1 << 24); //start conversion
    while(!(LPC_ADC->ADDR5 & 0x80000000));  //wait until conversion is done
    return (LPC_ADC->ADDR5 >> 4) & 0xFFF;   //get the 12 bit result
}
int main(){
    unsigned long adcVal;
    unsigned int i;
    float Vin;
    char volt[20], dval[20];
    unsigned char msg3[] = "Analog I/p: ";
    unsigned char msg4[] = "ADC o/p: ";
    SystemInit();
    SystemCoreClockUpdate();
    ADCinit();

    lcd_init();
    lcd_comdata(0x80, 0);
    delay_lcd(800);
    lcd_puts(msg3);
    lcd_comdata(0xC0, 0);
    delay_lcd(800);
    lcd_puts(msg4);

    while(1){
        adcVal = readADC(5);
        Vin = (((float) adcVal * Vref)) / (float) full_scale; //calculate voltage
        sprintf(volt, "%.2f V", Vin);
        sprintf(dval, "%lu", adcVal);
        // for(i = 0; i < 2000; i++);
        lcd_comdata(0x89, 0);
        delay_lcd(800);
        lcd_puts(volt);

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