#include <LPC17xx.h>
#include <stdio.h>

#define Ref_Vtg     3.300
#define Full_Scale  0xFFF  // 12-bit ADC = 4095

unsigned long adc_temp1 = 0, adc_temp2 = 0;
float in_vtg1 = 0, in_vtg2 = 0, diff = 0;
unsigned char vtg1[7], vtg2[7], vtg_diff[7];
unsigned char Msg3[6] = {"CH 4:"};
unsigned char Msg4[6] = {"CH 5:"};

void delay_lcd(unsigned int r1) {
    for (unsigned int r = 0; r < r1; r++);
}

void clear_ports(void) {
    LPC_GPIO0->FIOCLR = 0x0F << 23;  // Clear data lines P0.23 to P0.26
    LPC_GPIO0->FIOCLR = 1 << 27;     // Clear RS
    LPC_GPIO0->FIOCLR = 1 << 28;     // Clear EN
}

void write(int temp2, int type) {
    clear_ports();
    LPC_GPIO0->FIOPIN = temp2;
    if (type == 0)
        LPC_GPIO0->FIOCLR = 1 << 27; // RS = 0 for command
    else
        LPC_GPIO0->FIOSET = 1 << 27; // RS = 1 for data

    LPC_GPIO0->FIOSET = 1 << 28;     // EN = 1
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = 1 << 28;     // EN = 0
}

void lcd_comdata(int temp1, int type) {
    int temp2 = temp1 & 0xF0;
    temp2 <<= 19;
    write(temp2, type);
    temp2 = temp1 & 0x0F;
    temp2 <<= 23;
    write(temp2, type);
    delay_lcd(1000);
}

void lcd_puts(unsigned char *buf1) {
    unsigned int i = 0;
    while (buf1[i] != '\0') {
        lcd_comdata(buf1[i], 1);
        i++;
        if (i == 16)
            lcd_comdata(0xC0, 0); // Move to next line
    }
}

void lcd_init() {
    LPC_PINCON->PINSEL1 &= 0xFC003FFF;  // P0.23-P0.26 GPIO
    LPC_GPIO0->FIODIR |= 0x0F << 23;    // Data lines P0.23-P0.26
    LPC_GPIO0->FIODIR |= 1 << 27;       // RS
    LPC_GPIO0->FIODIR |= 1 << 28;       // EN

    clear_ports();
    delay_lcd(3200);

    lcd_comdata(0x33, 0); delay_lcd(30000);
    lcd_comdata(0x32, 0); delay_lcd(30000);
    lcd_comdata(0x28, 0); delay_lcd(30000);  // 4-bit, 2-line
    lcd_comdata(0x0C, 0); delay_lcd(800);    // Display ON, Cursor OFF
    lcd_comdata(0x06, 0); delay_lcd(800);    // Entry mode
    lcd_comdata(0x01, 0); delay_lcd(10000);  // Clear screen
}

void ADC_IRQHandler(void) {
    unsigned long status = LPC_ADC->ADSTAT;

    if (status & (1 << 4)) { // CH4 ready
        adc_temp1 = (LPC_ADC->ADDR4 >> 4) & 0xFFF;
    }

    if (status & (1 << 5)) { // CH5 ready
        adc_temp2 = (LPC_ADC->ADDR5 >> 4) & 0xFFF;

        // Calculate voltages and difference
        in_vtg1 = ((float)adc_temp1 * Ref_Vtg) / Full_Scale;
        in_vtg2 = ((float)adc_temp2 * Ref_Vtg) / Full_Scale;
        diff = in_vtg1 - in_vtg2;
        if (diff < 0) diff = -diff;

        // Format voltages
        sprintf((char *)vtg1, "%3.2fV", in_vtg1);
        sprintf((char *)vtg2, "%3.2fV", in_vtg2);
        sprintf((char *)vtg_diff, "%3.2fV", diff);

        // Display voltages and difference
        lcd_comdata(0x89, 0); delay_lcd(800);  // 2nd row, 2nd col
        lcd_puts(vtg1);
        lcd_comdata(0xC8, 0); delay_lcd(800);  // 3rd row, 9th col
        lcd_puts(vtg2);
        lcd_comdata(0x94, 0); delay_lcd(800);  // 4th row
        lcd_puts("Diff:");
        lcd_puts(vtg_diff);
    }
}

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    // LCD initialization
    lcd_init();
    lcd_comdata(0x80, 0); delay_lcd(800);
    lcd_puts(Msg3);
    lcd_comdata(0xC0, 0); delay_lcd(800);
    lcd_puts(Msg4);

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