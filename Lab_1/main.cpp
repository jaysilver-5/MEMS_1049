/*
 * lab1_2019.cpp
 *
 * Created: 1/9/2019 12:29:02 PM
 * Author : WWClark
 */ 

/*
This program is a solution to Lab 1 -- 
	Pushbutton switches are connected to Pins PD3 and PD4 (pulled high when switch is open).
	LEDs are connected to Pins PD6 and PD7 (active low). 
	
	The program causes the LEDs to mimic the input pin states (i.e. when Pin PD3 is high, the LED connected to Pin PD6 is on.  Likewise for Pins PD4 and PD7).

*/

#include <avr/io.h>

int main(void)
{
	 	
	DDRD = 0xE0;  // set bits PD7-PD5 as output, PD4-PD0 as input (this covers Pins 3 and 4 as needed for inputs, and 6 and 7 as needed for outputs)
	PORTD = 0b11111111; // set all bits on PORTD so that all output are off (assumes circuit is wired as active low)
		 
    // ============================================
    // P R O G R A M L O O P
    // ============================================
	while(1)
    {
				
       	if (PIND & 0b00001000) // "Read" the contents of PortD3 using bit-wise operation (AND PIND with binary 8 so that only PD3 is read)
		   // When read  as 1 (open switch) clear bit PD6 to turn on the LED.
		{
			PORTD = PORTD & 0b10111111; // Clear PD6 (which is active low),  leave other bits unchanged  (AND PIND with a binary number that has 0 in the only bit that we want to ensure is 0)
		}
		else 
		{
			PORTD = PORTD | 0b01000000;  // Set bit PD6 to turn off the  LED (which is active low), leave other bits unchanged (OR PIND with a binary number that has 1 in the only bit that we want to ensure is 1)
		}
		
		if (PIND & 0b00010000) // "Read" the contents of PortD4 using bit-wise operation (AND PIND with binary 16 so that only PD4 is read)
		{
			PORTD = PORTD & 0b01111111;  // Clear PD7, leave other bits unchanged (AND PIND with a binary number that has 0 in the only bit that we want to ensure is 0)
		}
		else 
		{
			PORTD = PORTD | 0b10000000; // Set PD7, leave other bits unchanged (OR PIND with a binary number that has 1 in the only bit that we want to ensure is 1)
		}
		
	}
	
return 0;	
}
// ============================================
// S U B R O U T I N E S
// ============================================



