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
        PORTC = PORTC ^ 1<<PORTC0;  // Note that this uses the Exclusive OR operator, which toggles an individual 
			// bit without changing the other bits in PORTC
                
        wait(250); // call delay function: the wait function will loop 250 time using delay #2 (1 msec each loop) for a total of 250 msec delay
         
    }
	
	return(0);
} // end main()

void wait(volatile int multiple) {
	// This subroutine creates a delay equal to multiple*T, where T is 1 msec
	while (multiple > 0) {
		TCCR0A = 0x00; // clears WGM00 and WGM01 (bits 0 and 1) to ensure Timer/Counter is in normal mode.
		TCNT0 = 0;  // preload value for testing on count = 250
		TCCR0B =  0b00000011; //1<<CS01 | 1<<CS00;	TCCR0B = 0x03;  // Start TIMER0, Normal mode, crystal clock, prescaler = 64
		while (TCNT0 < 0xFA); // exits when count = 250 (requires preload of 0 to make count = 250)
		TCCR0B = 0x00; // Stop TIMER0
		multiple--;
	}
} // end wait()