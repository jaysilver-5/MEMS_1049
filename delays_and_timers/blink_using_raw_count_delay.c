/*
 * blink_using_raw_count_delay.c
 *
 * Created: 1/20/2018 10:59:37 AM
 * Author : WW Clark
 */ 

/* 
 *
  This program is meant to blink an LED at a set frequency determined by a raw count loop to kill time, but the time is incorrect.
  Since it is C code, the exact number of 
  clock cycles used per instruction is not exactly known, so the times are approximate.
  
  ** Note that the loop takes much longer than expected because of many more instructions needed in assembly than expected.  See below.
  
  Connect LED to PC0.
  
  Connect LED signal to oscilloscope to test frequencies.
*
 */ 

#include <avr/io.h>

//** Functions **//
void wait(volatile int N);  // Note that variables in the delay loops are declared as volatile.  If not, the compiler will likely
	// see these as not necessary since they are not used for any important tasks or I/O and will eliminate the loops.  In fact, if 
	// the simulator is used without these being declared volatile, then the loops will be ignored all together in the simulation, 
	// and the cycle counter will not grow correctly.
void delay_1_msec_raw_count();

//** GLOBAL VARIABLES **/
long freqclk = 16000000; // clock frequency in Hz

int main(void)
{
	// Setup
	DDRC = 0xFF; // Sets all pins of PORTC to output
	PORTC = 0xFF; // Turn off all pins on PORTC (they are wired as active low)

   while(1)
    {
        // The main loop -- toggle the bit, then delay to set the frequency
       PORTC = PORTC ^ 1<<PORTC0;  // Note that this uses the Exclusive OR operator, which toggles an individual 
			// bit without changing the other bits in PORTC
                
        wait(250); // call delay function to delay a certain number of msec
		         
    }
} // end main()

/* ============================================
  S U B R O U T I N E S
 ============================================
*/


void wait(volatile int N) {
	//*** wait ***
		
	/* This subroutine creates a delay of N msec by calling another function.
			Inputs: N = number of msec for the delay (N is int, so can be up to 65,536 msec)
			Outputs: None
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
			// there is a lot more that needs to be done each cycle so the loop takes much longer than 1 msec to execute. 
			count--; // decrement count
	}
	
		
} // end delay_1_msec_raw_count()

