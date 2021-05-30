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
volatile int door1_count = 0;
volatile int door2_count = 0;
char door1_is_open_flag = 0;
char door2_is_open_flag = 0;

//** FUNCTION DECLARATIONS **//
void wait(volatile int N);  // Note that variables in the delay loops are declared as volatile.  If not, the compiler will likely
// see these as not necessary since they are not used for any important tasks or I/O and will eliminate the loops.  In fact, if
// the simulator is used without these being declared volatile, then the loops will be ignored all together in the simulation,
// and the cycle counter will not grow correctly.
void delay_1_msec_raw_count();

#include <avr/io.h>

int main(void)
{
	DDRD = 0b11000000;  // set bits PD7-PD6 as output, PD5-PD0 as input (this covers Pins 3 and 4 as needed for inputs, and 6 and 7 as needed for outputs)
	PORTD = 0b11000000; // set all output bits on PORTD so that all output are off (assumes circuits are wired as active low)
		 
    // ============================================
    // P R O G R A M L O O P
    // ============================================
	while(1)
    {
	    
	    if (PIND & 0b00001000) // Check the status of Switch 1.
	    // This line checks the switch status by doing a Boolean operation between PIND (where the switch is connected) and a binary number that targets the
	    // single pin that we are trying to read (Pin 3). For more on Boolean operations and "single bit addressing", see the C Programming slides.
	    // The argument of the If is TRUE only if the switch is closed (and Pin3 is a high voltage), in which case we want to turn on LED 1.
	    {
		    PORTD = PORTD & 0b10111111; // Turn on LED 1.
		    // This line uses more Boolean arithmetic to clear pin PD6 while leaving the other bits on PORTD unchanged.  Specifically, the line computes
		    // a logical AND between the existing PORTD values and a binary number that has 0 in the only bit that we want to ensure is 0, PD6.  All other bits remain
		    // unchanged (i.e. this line does not affect bit PD7, the other LED).
		    // The result is that since the LED on pin 6 of PORTD is wired as active low, this line causes it to turn on.
			if (door1_is_open_flag == 0) // only want to change count and use debounce the first time we see the switch open
			{
				door1_count++; // Increment count on door opening
				door1_is_open_flag = 1;
				wait(50); // debounce delay
			}
	    }
	    else
	    {
		    PORTD = PORTD | 0b01000000;  // Turn off LED 1.
		    // More Boolean arithmetic to cause PD6 to go high (a logical OR is performed between data in PORTD and a binary number that targets pin PD6),
		    // which causes LED 1 to be off and other bits, specifically LED 2, are unchanged.
			if (door1_is_open_flag == 1) // only want to use debounce the first time we see the switch close
			{
				door1_is_open_flag = 0;
				//wait(50); // debounce delay
			}
	    }
	    // Repeat for LED and Switch 2
	    if (PIND & 0b00010000) // Check the status of Switch 2.
	    // Same as above but pin PD4 is the target.
	    {
		    PORTD = PORTD & 0b01111111;  // Turn on LED 2.
		    // Same as in first If but pin PD7 is the target.
			if (door2_is_open_flag == 0) // only want to change count and use debounce the first time we see the switch open
			{
				door2_count++; // Increment count on door opening
				door2_is_open_flag = 1;
				wait(50); // debounce delay
			}
	    }
	    else
	    {
		    PORTD = PORTD | 0b10000000; // Turn off LED 2.
		    // Same as in first Else but pin PD7 is the target.
			if (door2_is_open_flag == 1) // only want to use debounce the first time we see the switch close
			{
				door2_is_open_flag = 0;
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
	volatile int i = (freqclk/1000-28)/4; // each cycle takes 4 clock cycles, so count is the number of cycles needed to create a 1 msec delay.
									// 28 is the approximate number of other clock cycles used in the subroutine
	while (i > 1) {  // This loop is expected to create a 1 msec delay, but it is in fact much longer.  Because of the size of the variables,
			// there is a lot more that needs to be done each cycle so the loop takes much longer than 1 msec to execute. To know exact time for each 
			// instruction, use assembly language instead of C. (Or look at the disassembly code file and count instructions.)
			i--; // decrement count
	}
} // end delay_1_msec_raw_count()

