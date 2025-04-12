#include<lpc17xx.h>
#include "LCDmsg.c"

#define BIT(n, x) (n << x)
int flag = 0;
unsigned char scan_code[16] = {
    0x11, 0x21, 0x41, 0x81, 
    0x12, 0x22, 0x42, 0x82, 
    0x14, 0x24, 0x44, 0x84,
    0x18, 0x28, 0x48, 0x88
};

unsigned char ascii_code[16] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'A', 'B',
    'C', 'D', 'E', 'F'
};

unsigned char scan(unsigned int rowpress){
    int colpress;
    unsigned char key;
    colpress = LPC_GPIO1->FIOPIN;
    colpress &= BIT(0xF, 23);
    if(colpress != 0x0){
        flag = 1;
        colpress >>= 19;        //23-4 bring from [26:23] to [7:4]
        rowpress >>= 10;        //10-0 bring from [13:10] to [3:0]
        key = colpress | rowpress;
    }
    return key;
}

int main(){
    unsigned char msg1[] = "Key Pressed: ";
    int i, row, rowpress;
    unsigned char key;
    SystemInit();
    SystemCoreClockUpdate();
    LPC_GPIO2->FIODIR |= BIT(0xF, 10);  //P2.10 to P2.13 as rows (o/p)
    LPC_GPIO1->FIODIR &= BIT(0xF, 23);  //P1.23 to P1.26 as cols (i/p)

    lcd_init();
    lcd_cmddata(0x80, 0);
    delay(800);
    lcd_puts(msg1);
    while(1){
        while(1){
            for(row = 1; row <= 4; row++){
                switch(row){
                    case 1: rowpress = 0x400;       //P2.10
                            break;
                    case 2: rowpress = 0x800;       //P2.11
                            break;
                    case 3: rowpress = 0x1000;      //P2.12
                            break;
                    case 4: rowpress = 0x2000;      //P2.13
                            break;
                }
                LPC_GPIO2->FIOCLR = BIT(0xF, 10);   //clear previous selection
                LPC_GPIO2->FIOSET = rowpress;       //activate row
                flag = 0;
                key = scan(rowpress);               //search for pressed column in the current row
                if(flag == 1) break;
            }
            if(flag == 1) break;
        }
        for(i = 0; i < 16; i++)
            if(key == scan_code[i]){
                key = ascii_code[i];
                break;
            }
        lcd_cmddata(0xc0, 0);
        delay(800);
        lcd_puts(&key);
    }
}