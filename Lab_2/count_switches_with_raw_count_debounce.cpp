/*
 * count_switches_with_raw_count_debounce.cpp
 *
 * Created: 5/18/2019 1:15:24 PM
 * Author : Buddy
 */ 

/*
This program adds to Lab 1 code that monitors switches (e.g. one on each of two door to check if it is open or closed) -- 
	Pushbutton switches are connected to Pins PD3 and PD4 (pulled high when switch is open).
	LEDs are connected to Pins PD6 and PD7 (active low). 
	
	The program causes the LEDs to mimic the input pin states (i.e. when Pin PD3 is high, the LED connected to Pin PD6 is on.  Likewise for Pins PD4 and PD7).
	For the switches on doors, the LEDs will be on when the door is open, and off when the door is closed.
	
	The addition to this program from Lab 1 is to count door openings.  We must add switch debouncing (done with delays created by raw counting in a for loop).

*/

//** GLOBAL VARIABLES **/
long freqclk = 16000000; // clock frequency in Hz
volatile int count_door1 = 0;
volatile int count_door2 = 0;
char flag1 = 0;
char flag2 = 0;

//** FUNCTION DECLARATIONS **//
void wait(volatile int N);  // Note that variables in the delay loops are declared as volatile.  If not, the compiler will likely
// see these as not necessary since they are not used for any important tasks or I/O and will eliminate the loops.  In fact, if
// the simulator is used without these being declared volatile, then the loops will be ignored all together in the simulation,
// and the cycle counter will not grow correctly.
void delay_1_msec_raw_count();

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
		//check "door 1" to see if it is open or closed
	   	if (PIND & 0b00001000) // Door 1 switch is open. "Read" the contents of PortD3 using bit-wise operation (AND PIND with binary 8 so that only PD3 is read)
		   // When read  as 1 (open switch) clear bit PD6 to turn on the LED.
		{
			PORTD = PORTD & 0b10111111; // Clear PD6 (which is active low),  leave other bits unchanged  (AND PIND with a binary number that has 0 in the only bit that we want to ensure is 0)
			if (flag1 == 0) // only want to change count and use debounce the first time we see the switch open
			{
				count_door1++; // Increment count on door opening
				flag1 = 1; 
				wait(50); // debounce delay
			}
		}
		else // Door 1 switch is closed.
		{
			PORTD = PORTD | 0b01000000;  // Set bit PD6 to turn off the  LED (which is active low), leave other bits unchanged (OR PIND with a binary number that has 1 in the only bit that we want to ensure is 1)
			if (flag1 == 1) // only want to use debounce the first time we see the switch close
			{
				flag1 = 0;
				wait(50); // debounce delay
			}
		}
		
		//check "door 2" to see if it is open or closed
		if (PIND & 0b00010000) // Door 2 switch is open.   "Read" the contents of PortD4 using bit-wise operation (AND PIND with binary 16 so that only PD4 is read)
		{
			PORTD = PORTD & 0b01111111;  // Clear PD7, leave other bits unchanged (AND PIND with a binary number that has 0 in the only bit that we want to ensure is 0)
			if (flag2 == 0) // only want to change count and use debounce the first time we see the switch open
			{
				count_door2++; // Increment count on door opening
				flag2 = 1;
				wait(50); // debounce delay
			}

		}
		else // Door 2 switch is closed.
		{
			PORTD = PORTD | 0b10000000; // Set PD7, leave other bits unchanged (OR PIND with a binary number that has 1 in the only bit that we want to ensure is 1)
			if (flag2 == 1) // only want to use debounce the first time we see the switch close
			{
				flag2 = 0;
				wait(50); // debounce delay
			}
		
		}
	}
	
return 0;	
} // end main

/* ============================================
  FUNCTIONS
 ============================================
*/

void wait(volatile int N) {
	//*** wait ***
		
	/* This subroutine creates a delay of N msec by calling another function.
			Inputs: N = number of msec for the delay (N is int, so can be up to 65,536 msec)
			Outputs: None
			
		Why is N a volatile variable? Some C compilers try to optimize the code and may eliminate code that appears to do nothing.  A time-killing 
		loop is is one of those code blocks that could be removed during compile.  Using a nonvolatile variable in the function tells the compiler not 
		remove it.
	*/
	while (N>1) {  // This loop creates an N msec delay
		N--;
		delay_1_msec_raw_count();
	}
} // end wait()

void delay_1_msec_raw_count() {
    //*** delay N ms **
    /* This subroutine creates a delay of N msec by using a raw time-killing count.
			Inputs: None
			Outputs: None
	*/
	volatile int count = (freqclk/1000-28)/4; // each cycle takes 4 clock cycles, so count is the number of cycles needed to create a 1 msec delay.
									// 28 is the approximate number of other clock cycles used in the subroutine
	while (count > 1) {  // This loop is expected to create a 1 msec delay, but it is in fact much longer.  Because of the size of the variables,
			// there is a lot more that needs to be done each cycle so the loop takes much longer than 1 msec to execute. To know exact time for each 
			// instruction, use assembly language instead of C. (Or look at the disassembly code file and count instructions.)
			count--; // decrement count
	}
	
		
} // end delay_1_msec_raw_count()

