#include <LPC17xx.h>
unsigned int i;
int LED;
int main(void){
	SystemInit();
	SystemCoreClockUpdate();
	
	LPC_PINCON -> PINSEL0 &= 0xFF0000FF;
	LPC_PINCON -> PINSEL1 &= 0xFFFFF3FF;
	LPC_GPIO0 -> FIODIR |= 0x00000FF0;
	LPC_GPIO0 -> FIODIR &= 0xFFDFFFFF;
	while(1){
		if( LPC_GPIO0 -> FIOPIN & 1 << 21){
			for(LED = 0; LED < 256; LED++){
				LPC_GPIO0 -> FIOPIN = LED << 4;
				for(i = 0; i < 100000; i++);
				if(!( LPC_GPIO0 -> FIOPIN & 1 << 21))
						break;
			}
		}
		else{
			for(LED = 255; LED >= 0; LED--){
				LPC_GPIO0 -> FIOPIN = LED << 4;
				for(i = 0; i < 100000; i++);
				if( LPC_GPIO0 -> FIOPIN & 1 << 21)
						break;
			}
		}
	}
}


// #include <LPC17xx.h>

// unsigned int counter = 0; // 8-bit counter

// void delay(){
// 	for (unsigned int i = 0; i < 100000; i++); // Simple delay
// }

// int main(){
// 	SystemInit();
// 	SystemCoreClockUpdate();

// 	LPC_PINCON->PINSEL0 &= 0xFF0000FF; // Configure P0.4 to P0.11 as GPIO
// 	LPC_GPIO0->FIODIR |= 0x00000FF0;   // Set P0.4 to P0.11 as output
// 	LPC_PINCON->PINSEL4 &= 0xFFCFFFFF; // Configure P2.12 as GPIO
// 	LPC_GPIO2->FIODIR &= ~(1 << 12);   // Set P2.12 as input

// 	while (1){
// 		if (LPC_GPIO2->FIOPIN & (1 << 12)){ // Check if switch is pressed
// 			counter++; // Increment counter
// 			if (counter > 255) counter = 0; // Wrap around at 8-bit limit
// 		} else {
// 			if (counter == 0) counter = 255; // Wrap around at 8-bit limit
// 			else counter--; // Decrement counter
// 		}

// 		LPC_GPIO0->FIOPIN = (counter << 4); // Output counter value to LEDs
// 		delay(); // Add delay for debouncing and visibility
// 	}
// }
