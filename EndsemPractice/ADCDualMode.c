#include<lpc17xx.h>
#include<stdio.h>
#include "LCDmsg.c"

#define Ref_Vtg 3.3
#define Full_Scale 0xFFF
#define BIT(n, x) (n << x)

float in_vtg1 = 0, in_vtg2 = 0, diff = 0;
unsigned char vtg1[7], vtg2[7], vtg_diff[7];

void delay_adc(unsigned int x){
    unsigned int i;
    for(i = 0; i < x; i++);
}

unsigned int read_software(unsigned int channel){
    LPC_ADC->ADCR = BIT(1, channel) | BIT(4, 8) | BIT(1, 21);//select ADC Channel, clkdiv and enable adc
    LPC_ADC->ADCR |= BIT(1, 24);                            //start conversion
    while(!(LPC_ADC->ADGDR & BIT(1, 31)));                    //wait for DONE 
    return (LPC_ADC->ADGDR >> 4) & 0xFFF;                   //extract result
}

void init_burst(){
    LPC_SC->PCONP |= BIT(1, 12);                            //power up ADC
    LPC_PINCON->PINSEL3 |= BIT(3, 28) | BIT(3, 30);   //P1.30 (AD0.4), P1.31 (AD0.5)
    LPC_ADC->ADCR = BIT(1, 4) | BIT(1, 5) | BIT(1, 16) | BIT(1, 21);    //Ch 4, Ch 5, burst mode, pdn 
    LPC_ADC->ADINTEN = BIT(1, 4) | BIT(1, 5);               //enable interrupt for Ch 4 and Ch 5
    NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_IRQHandler(){
    unsigned long status = LPC_ADC->ADSTAT;
    unsigned long res1, res2;
    if(status & BIT(1, 4))                                  //Ch 4 ready
        res1 = (LPC_ADC->ADDR4 >> 4) & 0xFFF;
    if(status & BIT(1, 5)){                                 //Ch 5 ready
        res2 = (LPC_ADC->ADDR5 >> 4) & 0xFFF;

        //convert to Voltages
        in_vtg1 = ((float)res1 * Ref_Vtg) / Full_Scale;
        in_vtg2 = ((float)res2 * Ref_Vtg) / Full_Scale;  
        diff = (in_vtg1 > in_vtg2) ? (in_vtg1 - in_vtg2) : (in_vtg2 - in_vtg1);

        sprintf((char*) vtg1, "%3.2fV", in_vtg1);
        sprintf((char*) vtg2, "%3.2fV", in_vtg2);
        sprintf((char*) vtg_diff, "%3.2fV", diff);

        lcd_cmddata(0x80, 0);
        delay(800);
        lcd_puts("Ch4: ");
        
        lcd_cmddata(0x85, 0);
        delay(800);
        lcd_puts(vtg1);

        lcd_cmddata(0xC0, 0);
        delay(800);
        lcd_puts("Ch5: ");

        lcd_cmddata(0xC5, 0);
        delay(800);
        lcd_puts(vtg2);

        lcd_cmddata(0x8A, 0);
        delay(800);
        lcd_puts("Diff: ");

        lcd_cmddata(0xCA, 0);
        delay(800);
        lcd_puts(vtg_diff);
    }
}

int main(){
    SystemInit();
    SystemCoreClockUpdate();

    lcd_init();
    delay(800);

    //burst mode
    init_burst();

    //software mode
    // unsigned int adc_sw = 0;
    // while(1){
    //     adc_sw = read_software(4);                          //read from AD0.4
    //     float voltage = ((float)adc_sw * Ref_Vtg) / Full_Scale;

    //     sprintf((char*)vtg1, "%3.2fV", voltage);

    //     lcd_cmddata(0xC0, 0);
    //     delay(800);
    //     lcd_puts("Ch4: ");

    //     lcd_cmddata(0xC8, 0);
    //     delay(800);
    //     lcd_puts(vtg1);
    //     delay_adc(5000000);
    // }
    while(1);
}