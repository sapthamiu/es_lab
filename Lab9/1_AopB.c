#include<lpc17xx.h>
#include "Lab8/LCDmessage.c"
unsigned char msg1[] = "Expression: ";
unsigned char row, key, flag;
unsigned long int var1, temp3;
char exp[5]; //A, op, B, =, \0
int index = 0;
unsigned char scan_code[16] = {
    0x11, 0x21, 0x41, 0x81,
    0x12, 0x22, 0x42, 0x82,
    0x14, 0x24, 0x44, 0x84,
    0x18, 0x28, 0x48, 0x88};
unsigned char ascii_code[16] = {
    '1', '2', '3', '+',
    '4', '5', '6', '-',
    '7', '8', '9', '=',
    '*', '0', '#', 'C'}; 
void scan(void){
    temp3 = LPC_GPIO1->FIOPIN & 0x07800000; //read column input
    if(temp3 != 0x00000000){
        flag = 1;
        temp3 >>= 19;
        key = temp3 | (var1 >> 10);
    }
}
char getKeypadInput(){
    while(1){
        flag = 0;
        for(row = 1; row <= 4; row++){
            switch(row){
                case 1: var1 = 0x00000400; break;
                case 2: var1 = 0x00000800; break;
                case 3: var1 = 0x00001000; break;
                case 4: var1 = 0x00002000; break;
            }
            LPC_GPIO2->FIOCLR = 0x00003C00;
            LPC-GPIO2->FIOSET = var1;
            scan();
            if(flag == 1) break;
        }
        if(flag == 1){
            for(int i = 0; i < 16; i++){
                if(key == scan_code[i])
                    return ascii_code[i];
            }
        }
    }
}
void evalexp(){
    int A = exp[0] - '0';
    char op = exp[1];
    int B = exp[2] - '0';
    int result = (op == '+') ? (A+B) : (A-B);
    char result[5];
    sprintf(result_str, "%d", result);
    lcd_comdata(0xC0);  //move to second line
    delay_lcd(800);
    lcd_puts(result);
}

int main(void){
    LPC_GPIO2->FIODIR |= 0x00003C00;    //P2.10 to P2.13 as output
    LPC_GPIO1->FIODIR &= ~(0x07800000); //P1.23 to P1.26 as input
    lcd_init();
    lcd_comdata(0x80);
    delay_lcd(800);
    lcd_puts(msg1);
    while(index < 4){   //read 4 characters
        char key = getKeypadInput();
        exp[index++] = key;
        char keystr[2] = {key, '\0'};
        lcd_puts(keystr);
        delay_lcd(500);
    }
    if(exp[3] == '=')
        evalexp();
    while(1);
}