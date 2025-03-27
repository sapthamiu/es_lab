/*#include<lpc17xx.h>
#include<stdio.h>
#include "lcdmsg.c"
char msg1[] = "Expression: ";
unsigned char row, flag, var, key, disp[3];
unsigned long int i, var1, temp, temp1temp3;
char express[5]; //A, op, B, =, \0
int ind = 0;
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
    if(temp3 != 0x0){
        flag = 1;
        temp3 >>= 19;
        key = temp3 | (var1 >> 10);
    }
}
char getKeypadInput(){
		int i;
    while(1){
        for(row = 1; row <= 4; row++){
            switch(row){
                case 1: var1 = 0x00000400; break;
                case 2: var1 = 0x00000800; break;
                case 3: var1 = 0x00001000; break;
                case 4: var1 = 0x00002000; break;
            }
            LPC_GPIO2->FIOCLR = 0x00003C00;
            LPC_GPIO2->FIOSET = var1;
						flag = 0;
            scan();
            if(flag == 1) break;
        }
        if(flag == 1) break;
				for(i = 0; i < 16; i++){
						if(key == scan_code[i])
								return ascii_code[i];
				}
    }
		return 0;
}
void lcd_puts(char* buf1){
    unsigned int i = 0;
    while(buf1[i] != '\0'){
        lcd_comdata(buf1[i], 1);
        i++;
        if(i == 16) //move cursor to 2nd line
            lcd_comdata(0xc0, 0);
    }
    return;
}
void evalexp(){
    int A = express[0] - '0';
    char op = express[1];
    int B = express[2] - '0';
    int result = (op == '+') ? (A+B) : (A-B);
    char resultstr[5];
    sprintf(resultstr, "%d", result);
    lcd_comdata(0xC0, 0);  //move to second line
    delay_lcd(800);
    lcd_puts(&resultstr[0]);
}

int main(void){
	int i;
    LPC_GPIO2->FIODIR |= 0x00003C00;    //P2.10 to P2.13 as output
    LPC_GPIO1->FIODIR &= ~(0x07800000); //P1.23 to P1.26 as input
    lcd_init();
    lcd_comdata(0x80, 0);
    delay_lcd(800);
    lcd_puts(msg1);
    while(ind < 4){   //read 4 characters
        while(1){
						for(row = 1; row <= 4; row++){
								switch(row){
										case 1: var1 = 0x00000400; break;
										case 2: var1 = 0x00000800; break;
										case 3: var1 = 0x00001000; break;
										case 4: var1 = 0x00002000; break;
								}
								LPC_GPIO2->FIOCLR = 0x00003C00;
								LPC_GPIO2->FIOSET = var1;
								flag = 0;
								scan();
								if(flag == 1) break;
						}
						if(flag == 1) break;
						for(i = 0; i < 16; i++){
								if(key == scan_code[i]){
										key = ascii_code[i];
										break;
								}
						}
				}
        express[ind++] = key;
        lcd_puts(&key);
        delay_lcd(500);
    }
    if(express[3] == '=')
        evalexp();
    while(1);
}
*/
#include <LPC17xx.h>
#include <stdlib.h>
#include "lcdmsg.c"

void scan(void);

unsigned char row, flag, key;
unsigned long int i, var1, temp, temp1 = 0, temp2, temp3;
unsigned char scan_code[16]={
	0x11, 0x21, 0x41, 0x81, 
	0x12, 0x22, 0x42, 0x82, 
	0x14,	0x24, 0x44, 0x84,
	0x18, 0x28, 0x48, 0x88 };
unsigned char ascii_code[16]={
	'0','1','2','3',
	'4','5','6','7',
	'8','9','A','B',
	'+','-','*','/' };
int idx = 1;
int ans = 0;
unsigned char finans[3] = {'0','0','\0'};
int a = 0;
int b = 0;
char op;
int count = 0;
int main(void)
{
	LPC_GPIO2->FIODIR = 0x3c00;
	LPC_GPIO1->FIODIR = 0xf87fffff;
	lcd_init();
	temp1 = 0x80;
	lcd_comdata(temp1,0);
	delay_lcd(80000);
	while(count<3)
	{
		while(1)
		{
			for(row=1; row<5; row++)
			{
				if(row==1)
					var1 = 0x400;
				else if(row==2)
					var1 = 0x800;
				else if(row==3)
					var1 = 0x1000;
				else if(row==4)
					var1 = 0x2000;
				temp = var1;
				LPC_GPIO2->FIOCLR = 0x3c00;
				LPC_GPIO2->FIOSET = var1;
				flag = 0;
				scan();
				if(flag==1)
				{
					count++;
					break;
				}
			}
			if(flag==1)
				break;
		}
		for(i=0; i<16; i++)
		{
			if(key==scan_code[i])
			{
				key = ascii_code[i];
				lcd_puts(&key);
				delay_lcd(100000);
				if(count==1)
					a = key-48;
				else if(count==2)
					op = key;
				else if(count==3)
					b = key-48;
				break;
			}
		}
	}
	temp1 = 0xc0;
	lcd_comdata(temp1,0);
	delay_lcd(800);
	switch(op)
	{
		case '+':
			ans = a+b;
			break;
		case '-':
			ans = a-b;
			break;
		case '*':
			ans = a*b;
			break;
		case '/':
			ans = a/b;
			break;
	}
	while(ans!=0)
	{
		finans[idx--] = (ans%10)+48;
		ans = ans/10;
	}
	lcd_puts(&finans[0]);
	return 0;
}

void scan(void)
{
	temp3 = LPC_GPIO1->FIOPIN;
	temp3 &= 0x07800000;
	if(temp3!=0)
	{
		flag = 1;
		temp3>>=19;
		temp>>=10;
		key = temp3|temp;
	}
}	
