#include<lpc17xx.h>
#include "lcdmsg.c"
unsigned char msg1[] = "KEY PRESSED=";
unsigned char row, var, flag, key;
unsigned long int i, var1, temp, temp1, temp2, temp3;
unsigned char scan_code[16] = {
    0x11, 0x21, 0x41, 0x81,
    0x12, 0x22, 0x42, 0x82,
    0x14, 0x24, 0x44, 0x84,
    0x18, 0x28, 0x48, 0x88};
unsigned char ascii_code[16] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'A', 'B',
    'C', 'D', 'E', 'F'};
void scan(void){
    temp3 = LPC_GPIO1->FIOPIN;
    temp3 &= 0x07800000; //check if any key is pressed in the enabled row
    if(temp3 != 0x0){
        flag = 1;
        temp3 >>= 19;   //shift to higher nibble of byte
        temp >>= 10;    //shift to lower nibble of byte
        key = temp3 | temp;     //get scan code //key = temp3 | (var1 >> 10) ?
    }
}

int main(void){
    LPC_GPIO2->FIODIR |= 0x00003C00;    //P2.10 to P2.13 (rows) as output
    LPC_GPIO1->FIODIR &= 0xF87FFFFF;    //P1.23 to P1.26 (cols) as input
    LPC_GPIO0->FIODIR |= 0x0F << 23 | 1 << 27 | 1 << 28;
    clear_ports();
    delay_lcd(3200);
    lcd_init();
    lcd_comdata(0x80,0);  //point to first line of LCD
    delay_lcd(800);
    lcd_puts(&msg1[0]);
    while(1){
			while(1){
        //flag = 0;
        for(row = 1; row < 5; row++){
            switch (row) {
                case 1: var1 = 0x00000400; break;
                case 2: var1 = 0x00000800; break;
                case 3: var1 = 0x00001000; break;
                case 4: var1 = 0x00002000; break;
            }
            temp = var1;
            LPC_GPIO2->FIOCLR = 0x00003C00;
            LPC_GPIO2->FIOSET = var1;   //enable the row
            flag = 0;
            scan();
            if(flag == 1)
                break;
        }
        if(flag == 1)
             break;
				for(i = 0; i < 16; i++)
						if(key == scan_code[i]){
								key = ascii_code[i];
								break;
						}
				lcd_comdata(0xc0,0);
				delay_lcd(800);
				lcd_puts(&key);
        }
    }
}
