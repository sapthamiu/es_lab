#include<lpc17xx.h>
void clear_ports(void){
    LPC_GPIO0->FIOCLR = 0x0F << 23; //clear data lines
    LPC_GPIO0->FIOCLR = 1 << 27;    //clear RS line
    LPC_GPIO0->FIOCLR = 1 << 28;    //clear Enable line
}
void delay_lcd(unsigned int r1){
    unsigned int r;
    for(r = 0; r < r1; r++);
}
void write(int temp2, int type){
    clear_ports();
    LPC_GPIO0->FIOPIN = temp2;  //assign value to the data lines
    if(type == 0)
        LPC_GPIO0->FIOCLR = 1 << 27;    //clear RS bit for command
    else 
        LPC_GPIO0->FIOSET = 1 << 27;    //set RS bit for data
    LPC_GPIO0->FIOSET = 1 << 28;    //enable = 1
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = 1 << 28; //enable = 0
    return;
}
void lcd_comdata(int temp1, int type){
    int temp2 = (temp1 & 0xF0) << 19;   // move data 26-8+1 times (higher nibble)
    write(temp2, type);
    temp2 = (temp1 & 0x0F) << 23;       //move data 26-4+1 times (lower nibble)
    write(temp2, type);
    delay_lcd(1000);
    return;
}
void lcd_init(void){
    LPC_PINCON->PINSEL1 &= ~(0x3FFC << 12); //P0.23 to P0.28 as GPIO
    LPC_GPIO0->FIODIR |= 0x0F << 23 | 1 << 27 | 1 << 28; //set direction as output for data lines, RS line and Enable line
    clear_ports();
    delay_lcd(3200);

    lcd_comdata(0x33, 0);
    delay_lcd(30000);
    lcd_comdata(0x32, 0);
    delay_lcd(30000);
    lcd_comdata(0x28, 0);   //function set - 4 bit, 2 line format
    delay_lcd(30000);
    lcd_comdata(0x0c, 0);   //display on, cursor off
    delay_lcd(800);
    lcd_comdata(0x06, 0);   //entry mode set increment cursor right
    delay_lcd(800);
    lcd_comdata(0x01, 0);   //display clear
    delay_lcd(10000);
    return;
}
void lcd_puts(unsigned char* buf1){
    unsigned int i = 0;
    while(buf1[i] != '\0'){
        lcd_comdata(buf1[i], 1);
        i++;
        if(i == 16) //move cursor to 2nd line
            lcd_comdata(0xc0, 0);
    }
    return;
}
int main(void){
    unsigned char msg1[4] = {"MIT"};
    unsigned char msg2[19] = {"Department of CSE"};
    SystemInit();
    SystemCoreClockUpdate();
    lcd_init();
    lcd_comdata(0x80, 0);
    delay_lcd(800);
    lcd_puts(&msg1[0]);
    lcd_comdata(0xC0, 0);
    delay_lcd(800);
    lcd_puts(&msg2[0]); 
}