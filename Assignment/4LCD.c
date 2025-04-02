#include<lpc17xx.h>
void clear_ports(){
    LPC_GPIO1->FIOCLR = 0x0F << 10;
    LPC_GPIO1->FIOCLR = 1 << 14;
    LPC_GPIO1->FIOCLR = 1 << 15;
}
void initTimer(unsigned int ms){
    LPC_TIM0->CTCR = 0x0;
    LPC_TIM0->TCR = 0x02;
    LPC_TIM0->PR = 119;
    LPC_TIM0->MR0 = ms * 99;  //delay in milliseconds
    LPC_TIM0->MCR = 0x02;
    LPC_TIM0->EMR = 0x20; 
    LPC_TIM0->TCR = 0x01;
}
void delay(unsigned int ms){
    initTimer(ms);
    while(LPC_TIM0->TC < LPC_TIM0->MR0);
}
void write(int value, int isData){
    clear_ports();
    LPC_GPIO1->FIOPIN = value;
    if(!isData)
        LPC_GPIO1->FIOCLR = 1 << 14;    //RS = 0 for command
    else 
        LPC_GPIO1->FIOSET = 1 << 14;    //RS = 1 for data
    LPC_GPIO1->FIOSET = 1 << 15;    //enable = 1
    delay(1);
    LPC_GPIO1->FIOCLR = 1 << 15;    //enable = 0
}
void lcd_comdata(int comdata, int isData){
	int upper, lower;
    upper = (comdata & 0xF0) << 6;    //higher nibble
    write(upper, isData);
    lower = (comdata & 0x0F) << 10;
    write(lower, isData);
    delay(1);
}

void lcd_init(){
    LPC_PINCON->PINSEL2 &= 0xFC003FFF;  //P1.10 to P1.15 as GPIO
    LPC_GPIO1->FIODIR |= (0x0F << 10) | (1 << 14) | (1 << 15);
    clear_ports();
    delay(32);  //power on delay

    lcd_comdata(0x33, 0);
    delay(30);
    lcd_comdata(0x32, 0);
    delay(30);
    lcd_comdata(0x28, 0);   //4-bit mode, 2-line display
    delay(30);
    lcd_comdata(0x0c, 0);   //display ON, cursor OFF
    delay(1);
    lcd_comdata(0x06, 0);   //entry mode, auto-increment cursor
    delay(1);
    lcd_comdata(0x01, 0);   //clear display
    delay(10);
}

void lcd_puts(unsigned char* msg){
    unsigned int i = 0;
    while(msg[i] != '\0'){
        lcd_comdata(msg[i], 1);
        i++;
        if(i == 16)
            lcd_comdata(0xc0, 0);   //move to 2nd line
    }
}

int main(){
    unsigned char msg[] = "Hello, world!";
    SystemInit();
    SystemCoreClockUpdate();
    lcd_init();
    lcd_comdata(0x80, 0);   //move cursor to first line
    delay(1);
    lcd_puts(msg);
    delay(5000);
    lcd_comdata(0x01, 0);   //clear LCD
    while(1);
}