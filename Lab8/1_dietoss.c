#include<lpc17xx.h>
#include<stdlib.h>
int die_roll = 1;
void clear_ports(void){
    LPC_GPIO0->FIOCLR = 0x0F << 23;
    LPC_GPIO0->FIOCLR = 1 << 27;
    LPC_GPIO->FIOCLR = 1 << 28;
}
void delay_lcd(unsigned int r1){
    unsigned int r;
    for(r = 0; r < r1; r++);
}
void write(int temp2, int type){
    clear_ports();
    LPC_GPIO0->FIOPIN = temp2;
    if(type == 0)   //command mode
        LPC_GPIO0->FIOCLR = 1 << 27;
    else            //data mode
        LPC_GPIO0->FIOSET = 1 << 27;
    LPC_GPIO0->FIOSET = 1 << 28;    //En = 1
    delay_lcd(50);
    LPC_GPIO0->FIOCLR = 1 << 28;    //En = 0
}
void lcd_comdata(int temp1, int type){
    int temp2 = (temp1 & 0xF0) << 19;
    write(temp2, type);
    temp2 = (temp1 & 0x0F) << 23;
    write(temp2, type);
    delay_lcd(1000);
}
void lcd_init(void){
    LPC_PINCON->PINSEL1 &= ~(0x3FFC << 12);
    LPC_GPIO0->FIODIR |= (0x0F << 23) | (1 << 27) | (1 << 28);
    clear_ports();
    delay_lcd(15000);
    lcd_comdata(0x33, 0);
    delay_lcd(5000);
    lcd_comdata(0x32, 0);
    delay_lcd(5000);
    lcd_comdata(0x28, 0);   //4 bit, 2 line format
    delay_lcd(5000);
    lcd_comdata(0x0C, 0);   //display on, cursor off
    delay_lcd(800);
    lcd_comdata(0x06, 0);   //entry mode
    delay_lcd(800);
    lcd_comdata(0x01, 0);   //clear display
    delay_lcd(3000);
}
void lcd_puts(unsigned char* str){
    int i = 0;
    while(str[i] != '\0'){
        lcd_comdata(str[i], 1);
        i++;
    }
}
void roll_die(void){
    die_roll = (rand() % 6) + 1;
    lcd_comdata(0x01, 0);   //clear screen
    delay_lcd(3000);
    lcd_comdata(0x80, 0);   //move cursor to first line
    lcd_puts("Die Toss: ");
    lcd_comdata(0xC0, 0);   //move cursor to 2nd line
    lcd_comdata(die_roll + '0', 1); //display after converting to char
}

int main(void){
    SystemInit();
    SystemCoreClockUpdate();
    lcd_init();
    LPC_PINCON->PINSEL4 &= ~(3 << 20);  //P2.10 as GPIO
    LPC_GPIO2->FIODIR &= ~(1 << 10);    //P2.10 as input
    lcd_comdata(0x80, 0);
    delay_lcd(800);
    lcd_puts("Press SW2");
    while(1){
        if(!(LPC_GPIO2->FIOPIN & (1 << 10))){
            delay_lcd(50000);
            if(!(LPC_GPIO2->FIOPIN & (1 << 10))){
                roll_die();
                while(!(LPC_GPIO2->FIOPIN & (1 << 10)));
            }
        }
    }
}