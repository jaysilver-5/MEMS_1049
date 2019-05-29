/*
 * blink_with_timer_interrupt.c
 *
 * Created: 3/16/2016 6:56:48 PM
 * Author : WW Clark
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
 
/* 
 *
  This program blinks an LED at set frequencies.  Connect LED to PC0.
  
  Uses TIMER0 register and interrupt on overflow.
    
  Connect LED signal to oscilloscope to test frequencies.
*
 */ 

//** Functions **//
void wait(volatile int multiple, volatile char time_choice);
void delay_T_msec_timer0(char choice);

//** GLOBAL VARIABLES **/
volatile int loop = 1;

//Interrupt Service Routine for Timer0
ISR(TIMER0_OVF_vect)
{
	TCCR0B = 0x00; // Stop TIMER0
	TIFR0 = 0x1<<TOV0;  // Clear TOV0 (note that this is an odd bit in that it
	//is cleared by writing a 1 to it)
	loop = 0;  // clear wait so that while loop exits 
}

int main(void)
{
	// Setup
	DDRC = 0xFF; // Sets all pins of PORTC to output
	PORTC = 0xFF; // Turn off all pins on PORTC (assume the pins are wired as active low)

	TIMSK0 = 1<<TOIE0;					// Enable TIMER0 overflow interrupt

	sei();				//Enable Global Interrupt
	
   while(1)
    {
       
        PORTC = PORTC ^ 1<<PORTC0;  // Toggle the LED
        //Note that this uses the Exclusive OR operator, which toggles an individual
        // bit without changing the other bits in PORTC        
         wait(250,2); // call delay function: the wait function will loop 250 time using delay #2 (1 msec each loop) for a total of 250 msec delay
		          
    }
} // end main()


void wait(volatile int multiple, volatile char time_choice) {
	/* This subroutine calls others to create a delay.
		 Total delay = multiple*T, where T is in msec and is the delay created by the called function.
	
		Inputs: multiple = number of multiples to delay, where multiple is the number of times an actual delay loop is called.
		Outputs: None
	*/
	
	while (multiple > 0) {
		delay_T_msec_timer0(time_choice); 
		multiple--;
	}
} // end wait()

void delay_T_msec_timer0(char choice) {
    //*** delay T ms **
    /* This subroutine creates a delay of T msec using TIMER0 with prescaler on clock, where, for a 16MHz clock:
    		T = .0156 msec for no prescaler and count of 250 (preload counter with 5)
    		T = 0.125 msec for prescaler set to 8 and count of 250 (preload counter with 5)
    		T = 1 msec for prescaler set to 64 and count of 250 (preload counter with 5)
    		T = 4 msec for prescaler set to 256 and count of 250 (preload counter with 5)
    		T = 16 msec for prescaler set to 1,024 and count of 250 (preload counter with 5)
	
			Inputs: None
			Outputs: None
	*/
	
	TCCR0A = 0x00; // clears WGM00 and WGM01 (bits 0 and 1) to ensure Timer/Counter is in normal mode.
	TCNT0 = 5;  // preload load TIMER0  (count must reach 255-5 = 250)
	
	switch ( choice ) { // choose prescaler
		case 1:
			TCCR0B = 1<<CS01;			//TCCR0B = 0x02; // Start TIMER0, Normal mode, crystal clock, prescaler = 8
		break;
		case 2:
			TCCR0B =  1<<CS01 | 1<<CS00;	//TCCR0B = 0x03;  // Start TIMER0, Normal mode, crystal clock, prescaler = 64
		break;
		case 3:
			TCCR0B = 1<<CS02;				//TCCR0B = 0x04; // Start TIMER0, Normal mode, crystal clock, prescaler = 256
		break; 
		case 4:
			TCCR0B = 1<<CS02 | 1<<CS00;	//TCCR0B = 0x05; // Start TIMER0, Normal mode, crystal clock, prescaler = 1024
		break;
		default:
			TCCR0B = 1<<CS00;				//TCCR0B = 0x01; Start TIMER0, Normal mode, crystal clock, no prescaler
		break;
	}
	
	while (loop); // wait for TOV0 to roll over and trigger TOV0 interrupt 
	
	loop = 1;  // prepare for next loop
	
} // end delay_T_msec_timer0()
