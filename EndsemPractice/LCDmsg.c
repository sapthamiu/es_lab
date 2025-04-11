#include<lpc17xx.h>
#define BIT(n, x) (n << x)

void clear_ports(){
    LPC_GPIO0->FIOCLR = BIT(0xF, 23) | BIT(1, 27) | BIT(1, 28); //data lines, RS line, enable line
}

void delay(int r){
    unsigned int i;
    for(i = 0; i < r; i++);
}

void write(int value, int type){
    clear_ports();
    LPC_GPIO0->FIOPIN = value;              //assign value to data lines
    if (type == 0) LPC_GPIO0->FIOCLR = BIT(1, 27);
		else LPC_GPIO0->FIOSET = BIT(1, 27);//set(data) or clear(cmd) RS 
    LPC_GPIO0->FIOSET = BIT(1, 28);         //rising edge
    delay(25);
    LPC_GPIO0->FIOCLR = BIT(1, 28);         //falling edge
}

void lcd_cmddata(int byte, int type){
		int high, low;
    high = (byte & 0xF0) << 19;         //23 - 4 : move higher nibble from [7:4] to [26:23]
    write(high, type);
    low = (byte & 0x0F) << 23;          //23 - 0 : move lower nibble from [3:0] to [26:23]
    write(low, type);
    delay(1000);
}

void lcd_init(){
    LPC_GPIO0->FIODIR |= BIT(0xF, 23) | BIT(1, 27) | BIT(1, 28);
    clear_ports();
    delay(3000);
    lcd_cmddata(0x33, 0);           //initialise
    delay(30000);
    lcd_cmddata(0x32, 0);           //initialise
    delay(30000);
    lcd_cmddata(0x28, 0);           //4 bit 2 line 5x7 dots
    delay(30000);
    lcd_cmddata(0x0c, 0);           //display on cursor off
    delay(800);
    lcd_cmddata(0x06, 0);           //entry mode
    delay(800);
    lcd_cmddata(0x01, 0);           //clear display
    delay(10000);
}

void lcd_puts(unsigned char* string){
    unsigned int i = 0;
    while(string[i] != '\0'){
        lcd_cmddata(string[i], 1);
        i++;
        if(i == 16) lcd_cmddata(0xc0, 0);
    }
}
/*int main(){
    unsigned char msg1[] = "MIT";
    unsigned char msg2[] = "Dept of CSE";
    SystemInit();
    SystemCoreClockUpdate();
    lcd_init();
    lcd_cmddata(0x80, 0);
    delay(800);
    lcd_puts(msg1);
    lcd_cmddata(0xC0, 0);
    delay(800);
    lcd_puts(msg2);
}
*/