/*
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
*/
#include <LPC17xx.h>
#include "LCDmsg.c"

#define BIT(n, x) (n << x)

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
    '+', '-', '*', '/'
};

unsigned char key = 0;
int a = 0, b = 0, ans = 0;
char op;
int count = 0, idx = 1;
unsigned char finans[3] = {'0', '0', '\0'};

unsigned char scan() {
    int row, colpress;
    unsigned int rowpress;
    unsigned char pressed = 0;

    for (row = 0; row < 4; row++) {
        LPC_GPIO2->FIOCLR = BIT(0xF, 10);        // Clear all rows
        rowpress = (1 << (10 + row));            // Activate one row
        LPC_GPIO2->FIOSET = rowpress;

        colpress = LPC_GPIO1->FIOPIN & BIT(0xF, 23);  // Read columns
        if (colpress != 0) {
            colpress >>= 19;
            rowpress >>= 10;
            pressed = colpress | rowpress;

            for (int i = 0; i < 16; i++) {
                if (pressed == scan_code[i])
                    return ascii_code[i];
            }
        }
    }
    return 0;
}

int main() {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_GPIO2->FIODIR |= BIT(0xF, 10);     // P2.10–P2.13 as output (rows)
    LPC_GPIO1->FIODIR &= ~BIT(0xF, 23);    // P1.23–P1.26 as input (columns)

    lcd_init();

    lcd_cmddata(0x80, 0);         // Move to first line
    lcd_puts("Expression: ");

    while (count < 3) {
        key = 0;
        while (key == 0)
            key = scan();         // Wait for keypress

        lcd_cmddata(0xC0 + count, 0);   // Move to position on second line
        lcd_cmddata(key, 1);            // Display character

        if (count == 0)
            a = key - '0';
        else if (count == 1)
            op = key;
        else if (count == 2)
            b = key - '0';

        count++;
        delay(100000);
    }

    // Evaluate the result
    switch (op) {
        case '+': ans = a + b; break;
        case '-': ans = a - b; break;
        case '*': ans = a * b; break;
        case '/': ans = a / b; break;
    }

    // Display result on LCD
    lcd_cmddata(0xC0 + count + 1, 0);  // Move to position after expression
    while (ans != 0 && idx >= 0) {
        finans[idx--] = (ans % 10) + '0';
        ans /= 10;
    }
    lcd_puts(finans);

    while (1);  // Infinite loop to freeze display
}
