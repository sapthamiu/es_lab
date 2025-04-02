/*Write an embedded C program to read input from 2*4 matrix of the keypad and to display 
the input value on SSD U8. The keypad is configured to read values from 0 to 7. 
Configure port pins 0.4 to 0.7 for columns of keypad, 0.8 to 0.11 for rows of keypad and 
1.15 to 1.18 as enable lines for SSDs and 2.0 to 2.7 as data lines for SSDs. 

0 1 2 3
4 5 6 7
*/
#include<lpc17xx.h>
unsigned char row, flag, key;
unsigned long int rowmask, colstatus;
unsigned char scan_code[8] = {
    0x11, 0x21, 0x41, 0x81,
    0x12, 0x22, 0x42, 0x82
};
unsigned char digits[8] = {
    0x3F, 0x06, 0x5B, 0x4F,
    0x66, 0x6D, 0x7D, 0x07
};
void scan(void){
    colstatus = LPC_GPIO0->FIOPIN & 0x000000F0; //status of P0.4 to P0.7 (cols) 
    if(colstatus != 0x00){   //if any key is pressed
        flag = 1;
        colstatus >>= 4;    //align column bits to higher nibble
        key = colstatus | rowmask; //get scan code
    }
}
void delay(int ms){
		int i;
    for(i = 0; i < ms * 5000; i++);
}
int main(void){
		int i;
    LPC_GPIO0->FIODIR |= 0x00000F00;    //P0.8 to P0.11 (rows) as output
    LPC_GPIO0->FIODIR &= ~0x000000F0;   //P0.4 to P0.7 (cols) as input
    LPC_GPIO1->FIODIR |= 0x00078000;    //P1.15 to P1.18 (SSD select lines)
    LPC_GPIO2->FIODIR |= 0x000000FF;    //P2.0 to P2.7 (SSD data lines)
    while(1){
        for(row = 1; row <= 2; row++){
            switch(row){
                case 1: rowmask = 0x00000100; break;   //P0.8
                case 2: rowmask = 0x00000200; break;   //P0.9
            }
            LPC_GPIO0->FIOCLR = 0x00000F00; //clear previous selection
            LPC_GPIO0->FIOSET = rowmask;   //enable the row
            flag = 0;
            scan();
            if(flag == 1) break;
        }
        if(flag == 1){
            for(i = 0; i < 8; i++){
                if(key == scan_code[i]){
                    LPC_GPIO1->FIOSET = 0x00008000;  //enable SSD P1.15 (U8)
                    LPC_GPIO2->FIOCLR = 0x000000FF; //clear SSD data
                    LPC_GPIO2->FIOSET = digits[i];
                    break;
                }
            }
        }
        delay(200);
    }
}