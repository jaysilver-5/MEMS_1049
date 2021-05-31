/*
 * blink_with_timer_interrupt.c
 *
 * Created: 5/17/2020
 * Author : WW Clark
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
 
/* 
 *
  This program blinks an LED at set frequencies.  The timing is handled completely by the Timer0 register in the background, and the microcontroller can be doing some other task.
  Connect LED to PC0.
  
  Uses TIMER0 register and interrupt on overflow.
    
  Connect LED signal to oscilloscope to test frequencies.
*
 */ 

//** GLOBAL VARIABLES **/
volatile int loop = 0;
volatile int loop_count_max = 31; // number of times the ISR needs to run to cause a 0.5 second delay  -- 31*16msec = 0.5 second to turn LED on or off
volatile int timer_preload = 5; // value for presetting timer to create precise delay

//Interrupt Service Routine for Timer0
ISR(TIMER0_OVF_vect)
{
	loop++;
	if (loop==loop_count_max) { // 1 second has elapsed -- 62*16msec = 1 second
		TCCR0B = 0x00; // Stop TIMER0
		TIFR0 = 0x1<<TOV0;  // Clear TOV0 (note that this is an odd bit in that it is cleared by writing a 1 to it)
		loop = 0; // reset count for another 1 second delay
		PORTC = PORTC ^ 1<<PORTC0;  // Toggle the LED //Note that this uses the Exclusive OR operator, which toggles an individual bit without changing the other bits in PORTC
		TCNT0 = timer_preload;  // preload load TIMER0
		TCCR0B = 1<<CS02 | 1<<CS00;	//TCCR0B = 0x05; // **re-start TIMER0, Normal mode, crystal clock, prescaler = 1024 (one cycle creates a 16 msec delay if count preloaded 5)
	}
}

int main(void)
{
	int i = 0; // dummy variable
	
	// Set up ports
	DDRC = 0xFF; // Sets all pins of PORTC to output
	PORTC = 0xFF; // Turn off all pins on PORTC (assume the pins are wired as active low)
	// Set up timer
	TCCR0A = 0x00; // clears WGM00 and WGM01 (bits 0 and 1) to ensure Timer/Counter is in normal mode.
	TIMSK0 = 1<<TOIE0;	// Enable TIMER0 overflow interrupt
	TCNT0 = timer_preload;  // preload load TIMER0 
	sei();	//Enable Global Interrupt
	TCCR0B = 1<<CS02 | 1<<CS00;	//TCCR0B = 0x05; // **Start TIMER0, Normal mode, crystal clock, prescaler = 1024 (one cycle creates a 16 msec delay if count preloaded 5)
		
   while(1)  // this is just a dummy loop that continually does some task while the timing and LED blinking is handled automatically in the background
	   // You can do any other tasks in this loop.
    {
       i++;
	   if (i == 1000) {
		   i = 0;
	   }		          
    }
} // end main()

