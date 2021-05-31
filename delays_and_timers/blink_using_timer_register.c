/*
 * blink_using_timer_register.c
 *
 * Created: 5/17/2020
 * Author : WW Clark
 */
 
/* 
 *
  This program blinks an LED at set frequencies.  Connect LED to PORTC0.
  
  Uses TIMER0 register at a single blink rate with a fixed timer prescaler.  To allow more general prescaler setting, see blink_with_timer_general_delay.c
    
  Connect LED signal to oscilloscope to test frequencies.
*
 */ 

#include <avr/io.h>

//** Functions **//
void wait(volatile int multiple);

//** GLOBAL VARIABLES **/
// none

int main(void)
{
	// Setup
	DDRC = 0xFF; // Sets all pins of PORTC to output
	PORTC = 0xFF; // Turn off all pins on PORTC (assume pins are configured as active low)

   while(1)
    {
        // The main loop -- toggle the bit, then delay to set the frequency
		PORTC = 0b00000001; // Turn off LED -- note that this also clears all other bits on PORTC
		wait(250);
		PORTC = 0b00000000; // Turn on LED --  note that this also clears all other bits on PORTC
		wait(250);
		
		// An alternate and efficient way to toggle the LED on and off is shown below with single bit addressing.  This avoids affecting other bits on PORTC besides the one we want to address.
        //PORTC = PORTC ^ 1<<PORTC0;  // Note that this uses the Exclusive OR operator, which toggles an individual bit without changing the other bits in PORTC
        //wait(250); // call delay function: the wait function will loop 250 time using delay #2 (1 msec each loop) for a total of 250 msec delay     
    }	
	return(0);
} // end main()

void wait(volatile int N) {
	// This subroutine creates a delay equal to N msec
	while (N > 0) {
		TCCR0A = 0x00; // clears WGM00 and WGM01 (bits 0 and 1) to ensure Timer/Counter is in normal mode.
		TCNT0 = 0;  // preload value for testing on count = 250
		TCCR0B =  0b00000011; //1<<CS01 | 1<<CS00;	TCCR0B = 0x03;  // Start TIMER0, Normal mode, 16 MHz crystal clock, prescaler = 64
		while (TCNT0 < 0xFA); // exits when count = 250 (which is the count required to create a 1 msec delay with 16MHz clock and prescaler of 64)
		TCCR0B = 0x00; // Stop TIMER0
		N--;
	}
} // end wait()