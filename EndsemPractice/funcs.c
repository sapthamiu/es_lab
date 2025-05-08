//aproximate delay
void delay(unsigned int ms){
	unsigned int i;
	for(i = 0; i < ms; i++);
}
//led bcd upcounter (P1.0 to P1.3)
void upcount(){
	int count;
	LPC_GPIO1->FIODIR |= 0x0F;
	for(count = 0; count <= 9; count++){
		LPC_GPIO1->FIOCLR = 0X0F;
		LPC_GPIO1->FIOSET = count & 0X0F;
		delay(30000);
	}
}
//led bcd downcounter (P1.0 to P1.3)
void downcount(){
	int count;
	LPC_GPIO1->FIODIR |= 0x0F;
	for(count = 9; count >= 0; count--){
		LPC_GPIO1->FIOCLR = 0x0F;
		LPC_GPIO1->FIOSET = count & 0X0F;
		delay(30000);
	}
}
//7sd digit display (P0.0 to P0.7)
unsigned char digit[] = {0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F};
void digitdisplay(){
	int count;
	LPC_GPIO0->FIODIR |= 0xFF;
	for(count = 0; count <= 9; count++){
		LPC_GPIO0->FIOCLR = 0x7F;
		LPC_GPIO0->FIOSET = digit[count];
		delay(1000);
	}
}
//7sd 4 digit display (P0.4 to P0.11 for data lines, P1.23 to P1.26 for select lines)
unsigned char digit[] = {0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X6F};
int position[] = {0, 1, 2, 3};
int content[] = {5, 2, 0, 2};
void digitsdisplay(){
	int pos;
	LPC_GPIO1->FIODIR |= 0x0F << 23;
	LPC_GPIO0->FIODIR |= 0xFF << 4;
	for(pos = 0; pos <= 3; pos++){
		LPC_GPIO1->FIOCLR = 0x0F << 23;
		LPC_GPIO1->FIOSET = position[pos] << 23;
		LPC_GPIO0->FIOCLR = 0xFF << 4;
		LPC_GPIO0->FIOSET = digit[content[pos]];
		delay(1000);
	}
}
//lcd display (P0.23 to P0.26 for data lines, P0.27 for register select (RS), P0.28 for enable(EN))
//latch data to lcd display
void write(int value, int type){
	LPC_GPIO0->FIOCLR = (0xF << 23) | (1 << 27) | (1 << 28);
	LPC_GPIO0->FIOPIN = value;
	if(type == 0) LPC_GPIO0->FIOCLR = 1 << 27;
	else LPC_GPIO0->FIOSET = 1 << 27;
	LPC_GPIO0->FIOSET = 1 << 28;
	delay(25);
	LPC_GPIO0->FIOCLR = 1 << 28;
}
//split the byte and send each nibble to write
void lcd_cmddata(int byte, int type){
	int high, low;
	high = (byte & 0xF0) << 19;
	write(high, type);
	low = (byte & 0x0F) << 23;
	write(low, type);
	delay(1000);
}
//initialise lcd
void lcd_init(){
	LPC_GPIO0->FIODIR |= (0x0F << 23) | (1 << 27) | (1 << 28);
	LPC_GPIO0->FIOCLR |= (0x0F << 23) | (1 << 27) | (1 << 28);
	delay(3000);
	lcd_cmddata(0x33, 0);
	delay(30000);
	lcd_cmddata(0x32, 0);
	delay(30000);
	lcd_cmddata(0x28, 0);
	delay(30000);
	lcd_cmddata(0x0C, 0);
	delay(30000);
	lcd_cmddata(0x06, 0);
	delay(30000);
	lcd_cmddata(0x01, 0);
	delay(30000);
}
//split the message and send each character to lcd_cmddata
void lcd_puts(unsigned char* string){
	unsigned int i = 0;
	while(string[i] != '\0'){
		lcd_cmddata(string[i++], 1);
		if(i == 16) lcd_cmddata(0xC0, 0);
	}
}

//4x4 keypad (P2.10 to P2.13 as rows-output, P1.23 to P1.26 as columns-input)
unsigned char scancode[] = {
	0x11, 0x21, 0x41, 0x81,
	0x12, 0x22, 0x42, 0x82,
	0x14, 0x24, 0x44, 0x84,
	0x18, 0x28, 0x48, 0x88
};
unsigned char asciicode[] = {
	'0', '1', '2', '3',
	'4', '5', '6', '7',
	'8', '9', 'A', 'B',
	'C', 'D', 'E', 'F'
};
unsigned char scan(){
	int row, i;
	unsigned int rowpress, colpress;
	unsigned char key = 0;
	for(row = 0; row < 4; row++){
		rowpress = 1 << (10 + row);
		LPC_GPIO2->FIOCLR = (0xF << 10);
		LPC_GPIO2->FIOSET = rowpress;
		colpress = LPC_GPIO1->FIOPIN & (0x0F << 23);
		if(colpress != 0x0){
			rowpress >>= 10;
			colpress >>= 19;
			key = colpress | rowpress;
			for(i = 0; i < 16; i++)
				if(key == scancode[i])
					return asciicode[i];
		}
	}
}
//stepper motor (P0.4 to P0.7)
void clockwise(){
	unsigned long int step = 1 << 4;
	unsigned int i;
	for(i = 0; i < 4; i++){
		LPC_GPIO0->FIOCLR = 0xF0;
		LPC_GPIO0->FIOSET = step;
		delay(3000);
		step <<= 1;
	}
}
void anticlockwise(){
	unsigned long int step = 1 << 7;
	unsigned int i;
	for(i = 0; i < 4; i++){
		LPC_GPIO0->FIOCLR = 0xF0;
		LPC_GPIO0->FIOSET = step;
		delay(3000);
		step >>= 1;
	}
}
void rotate(){
	int j;
	for(j = 0; j < 50; j++)
		clockwise();
	delay(65000);
	for(j = 0; j < 50; j++)
		anticlockwise();
	delay(65000);
}
//timer 0
void init_timer0(){
	LPC_TIM0->CTCR = 0x0;	//timer mode
	LPC_TIM0->PR = 2;		//TC increments every 3 Pclk cycles
	LPC_TIM0->MR0 = 999;	//1 ms delay
	LPC_TIM0->MCR = 2; 		//reset TC on match
	LPC_TIM0->EMR = (0x10 << 4);//set bit 0 of EMR high on TC match
	LPC_TIM0->TCR = 0x02;	//reset timer
	LPC_TIM0->TCR = 0x01;	//enable timer
}
void delay_ms(){
	init_timer0();
	while(!(LPC_TIM0->EMR & 1));
}
//UART0 (P0.2 for TxD, P0.3 for RxD)
void initUART0(){
	LPC_SC->PCONP |= 1 << 3;	//power up UART
	LPC_UART0->LCR |= 0x0011;	//8 bits (11), no parity(0), 1 stop bit (0) 
	LPC_UART0->LCR |= 1 << 7;	//set DLAB = 1
	LPC_UART0->DLL = 0x0;		//set baud rate
	LPC_UART0->DLM = 0;
	LPC_UART0->FDR = (1 << 4) | (1 << 6);//MULVAL = 2 (bits 4 to 7), DIVADDVAL = 1 (bits 0 to 3)
	LPC_UART0->FCR = 1 << 0 | 1 << 1 | 1 << 2;//enable FIFO
	LPC_PINCON->PINSEL |= (1 << 4) | (1 << 6);//P0.2, P0.3 for TxD and RxD
	LPC_UART0->LCR &= 0 << 7;
}
void U0_write(char txData){
	while(!(LPC_UART0->LSR & (1 << 5)));	//wait until THR is empty
	LPC_UART0->THR = txData;				//write to THR
}
char U0_read(){
	while(!(LPC_UART0->LSR & (1 << 0)));	//wait until data arrives in RBR
	return LPC_UART0->RBR;
}
//ADC
//software mode (AD0.0)
float init_software(){
	int result;
	float volt = 0;
	LPC_PINCON->PINSEL1 = (1 << 14); 					//P0.23 for AD0.0
	LPC_SC->PCONP |= (1 << 12);							//enable ADC clock
	LPC_ADC->ADCR = (1 << 0) | (1 << 8) | (1 << 21) ;	//select channel, clkdiv and enable 
	LPC_ADC->ADCR |= (1 << 24); 						//start conversion
	while (!(LPC_ADC->ADGDR & (1 << 31));				//wait for conversion to complete
	result = (LPC_ADC->ADGDR >> 4) & 0xFFF;				//extract result
	volt = ((float)result * 3.3) / 0xFFF;			//convert to Volt 
	return volt;
}
//burst mode (AD0.0, AD0.1)
void init_burst(){
	LPC_PINCON->PINSEL |= (1 << 14) | (1 << 16);							//P0.23 for AD0.0, P0.24 for AD0.1
	LPC_SC->PCONP |= (1 << 12);
	LPC_ADC->ADCR = (1 << 0) | (1 << 1) | (1 << 8) | (1 << 16) | (1 << 21);	//select channels, clkdiv, burst and enable
	LPC_ADC->ADINTEN = (1 << 0) | (1 << 1);
	NVIC_EnableIRQ(ADC_IRQn);
}
//ISR to return difference of the two voltages
void ADC_IRQHandler(){
	int result0, result1;
	float volt0 = 0, volt1 = 0;
	if(LPC_ADC->ADSTAT & (1 << 0))
		result0 = (LPC_ADC->ADDR0 >> 4) & 0xFFF;
	if(LPC_ADC->ADSTAT & (1 << 1)){
		result1 = (LPC_ADC->ADDR1 >> 4) & 0xFFF;
		volt1 = ((float)result1 * 3.3) / 0xFFF;
		volt0 = ((float)result0 * 3.3) / 0xFFF;
	}
	return (volt1 > volt0) ? (volt1 - volt0) : (volt0 - volt1);
}
//DAC
void delayms(unsigned int ms){
	LPC_TIM0->CTCR = 0x0;		//timer mode
	LPC_TIM0->PR = 2;			//TC increments every 3 Pclk cycles
	LPC_TIM0->TCR = 0x02;		//reset timer
	LPC_TIM0->TCR = 0x01;		//enable timer
	while(LPC_TIM0->TC < ms);	//delay
	LPC_TIM0->TCR = 0x00;		//disable timer
}
//sawtooth waveform 
void sawtooth(){
	unsigned int value = 0;
	LPC_PINCON->PINSEL1 |= (0x10 << 20);	//P0.26 as AOUT 
	while(1){
		if(value > 1023) value = 0;
		LPC_DAC->DACR = (value << 6);
		delayms(9999);
		value++;
	}
}
//triangular waveform 
void triangular(){
	unsigned int value;
	LPC_PINCON->PINSEL1 |= (0x10 << 20); 
	for(value = 0; value <= 1023; value++)
		LPC_DAC->DACR = (value << 6);
	for(value = 1023; value >= 0; value--)
		LPC_DAC->DACR = (value << 6);
}
//square waveform
void square(){
	unsigned int value;
	LPC_PINCON->PINSEL1 |= (0x10 << 20);
	while(1){
		LPC_DAC->DACR = (1023 << 6);
		delayms(9999);
		LPC_DAC->DACR = (0 << 6);
		delayms(9999);
	}
}
//sine waveform 
void sine(){
	unsigned int deg;
	float rad, value;
	LPC_PINCON->PINSEL1 |= (0x10 << 20);
	while(1){
		for(deg = 0; deg <= 360; deg += 9){		//considering we need 36 samples for one cycle
			rad = sin(deg * (3.14/180));		//convert to radian
			value = 1.65 + 1.65 * rad;			//find amplitude
			value = (value * 1023) / 3.3;		//scale to 10 bits (1023)
			LPC_DAC->DACR = (value << 6);
		}
	}
}
//PWM (P1.18 for P1.1)
void initPWM(){
	LPC_PINCON->PINSEL3 |= (1 << 4);				//P1.18 (01) as PWM1.1
	LPC_PWM1->PR = 24;								//1 microsecond resolution at 25MHz
	LPC_PWM1->MR0 = 10000;							//10ms period
	LPC_PWM1->MR1 = 0;								//start with 0% duty 
	LPC_PWM1->MCR = (1 << 1);						//reset TC on MR0 match
	LPC_PWM1->LER = (1 << 1) | (1 << 1);			//enable MR0 and MR1 updates
	LPC_PWM1->PCR = (1 << 9);						//enable PWM1.1 output 
	LPC_PWM1->TCR = (1 << 1);						//reset timer
	LPC_PWM1->TCR = (1 << 0) | (1 << 3);			//enable timer and PWM outputs
}


void initPWM(){
    LPC_PINCON->PINSEL3 |= BIT(1, 15);  //P1.23 (01) as PWM1.4 
    LPC_PWM1->PCR = BIT(1, 12);         //enable PWM1.4 o/p
    LPC_PWM1->PR = 0;                   //no prescaler (Pclk = Tclk)
    LPC_PWM1->MR0 = 30000;              //PWM period = 10ms 
    LPC_PWM1->MR4 = 0;                  //start with 0% duty
    LPC_PWM1->MCR = BIT(1, 1);          //reset on MR0 match
    LPC_PWM1->LER = BIT(1, 0) | BIT(1, 4);//load MR0 and MR4
    LPC_PWM1->TCR = BIT(1, 1);          //reset TC and PC
    LPC_PWM1->TCR = BIT(1, 0) | BIT(1, 3);//enable TC and PC
}

void updatePW(unsigned int pw){
    LPC_PWM1->MR4 = pw;                 //update MR with new duty value
    LPC_PWM1->LER = BIT(1, 4);          //load MR4 value on next cycle
}

void delayms(unsigned int ms){
    LPC_TIM0->CTCR = 0x0;               //timer mode
    LPC_TIM0->PR = 2;                   //increment TC at every 3 cycles
    LPC_TIM0->TCR = 0x02;               //reset timer
    LPC_TIM0->TCR = 0x01;               //start timer
    while(LPC_TIM0->TC < ms);           //wait
    LPC_TIM0->TCR = 0x00;               //stop timer
}

int main(){
    int pws[] = {0, 3000, 6000, 9000, 12000, 15000, 18000, 21000, 24000, 27000};
    const int n = 10;    
    initPWM();

    while (1) {
        // Increasing brightness
        for (int i = 1; i < n; i++) {
            updatePW(pws[i]);
            delayms(500);
        }

        // Decreasing brightness
        for (int i = n - 2; i > 0; i--) {
            updatePW(pws[i]);
            delayms(500);
        }
    }
}