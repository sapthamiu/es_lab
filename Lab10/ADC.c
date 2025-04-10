#include <LPC17xx.h>
#include <stdio.h>
#include "LCDmessage.c"
#define Ref_Vtg     3.300
#define Full_Scale  0xFFF  // 12-bit ADC = 4095

unsigned long temp1 = 0, temp2 = 0;
float in_vtg1 = 0, in_vtg2 = 0, diff = 0;
unsigned char vtg1[7], vtg2[7], vtg_diff[7];
unsigned char Msg5[6] = "Diff:";
void ADC_IRQHandler(void) {
    unsigned long status = LPC_ADC->ADSTAT;

    if (status & (1 << 4))  // CH4 ready
        temp1 = (LPC_ADC->ADDR4 >> 4) & 0xFFF;

    if (status & (1 << 5)) { // CH5 ready
        temp2 = (LPC_ADC->ADDR5 >> 4) & 0xFFF;

        // Calculate voltages and difference
        in_vtg1 = ((float)temp1 * Ref_Vtg) / Full_Scale;
        in_vtg2 = ((float)temp2 * Ref_Vtg) / Full_Scale;
        diff = in_vtg1 - in_vtg2;
        if (diff < 0) diff = -diff;

        // Format voltages
        sprintf((char *)vtg1, "%3.2fV", in_vtg1);
        sprintf((char *)vtg2, "%3.2fV", in_vtg2);
        sprintf((char *)vtg_diff, "%3.2fV", diff);

        // Display voltages and difference
        lcd_comdata(0xC8, 0); delay_lcd(800);  // 2nd row, 2nd col
				lcd_puts(vtg_diff);
        lcd_comdata(0x80, 0); delay_lcd(800);  // 3rd row, 9th col
				lcd_puts(vtg1);
			  lcd_comdata(0x89, 0); delay_lcd(800);  // 3rd row, 9th col
        lcd_puts(vtg2);
			
        //lcd_comdata(0x94, 0); delay_lcd(800);  // 4th row
        //lcd_puts("Diff:");
       
    }
}

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    // LCD initialization
    lcd_init();
    lcd_comdata(0xC0, 0); delay_lcd(800);
    lcd_puts(Msg5);
  
    // Enable ADC power
    LPC_SC->PCONP |= (1 << 12);

    // Set pins P1.30 (CH4) and P1.31 (CH5) as AD0.4 and AD0.5
    LPC_PINCON->PINSEL3 |= (3 << 28) | (3 << 30);

    // ADC Control Register:
    // SEL = CH4 & CH5, BURST = 1 (bit 16), PDN = 1 (bit 21)
    LPC_ADC->ADCR = (1 << 4) | (1 << 5) | (1 << 16) | (1 << 21);

    // Enable interrupts for CH4 and CH5
    LPC_ADC->ADINTEN = (1 << 4) | (1 << 5);
    NVIC_EnableIRQ(ADC_IRQn);  // Enable interrupt in NVIC

    while (1); // All work is done in the interrupt handler
}