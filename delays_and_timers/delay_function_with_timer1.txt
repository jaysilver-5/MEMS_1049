void delay_T_msec_timer1(volatile char choice) {
    //  
// ***Note that since the Timer1 register is 16 bits, the delays can be much higher than shown here.
    // This subroutine creates a delay of T msec using TIMER1 with prescaler on clock, where, for a 16MHz clock:
    //T = 0.125 msec for prescaler set to 8 and count of 250 (preload counter with 65,535-5)
    //T = 1 msec for prescaler set to 64 and count of 250 (preload counter with 65,535-5)
    //T = 4 msec for prescaler set to 256 and count of 250 (preload counter with 65,535-5)
    //T = 16 msec for prescaler set to 1,024 and count of 250 (preload counter with 65,535-5)
//Default: T = .0156 msec for no prescaler and count of 250 (preload counter with 65,535-5)

//Inputs: None
//Outputs: None

TCCR1A = 0x00; // clears WGM00 and WGM01 (bits 0 and 1) to ensure Timer/Counter is in normal mode.
TCNT1 = 0;  // preload load TIMER1 with 5 if counting to 255 (count must reach 65,535-5 = 250)
// or preload with 0 and count to 250

switch ( choice ) { // choose prescaler
case 1:
TCCR1B = 1<<CS11;//TCCR1B = 0x02; // Start TIMER1, Normal mode, crystal clock, prescaler = 8
break;
case 2:
TCCR1B =  1<<CS11 | 1<<CS10;//TCCR1B = 0x03;  // Start TIMER1, Normal mode, crystal clock, prescaler = 64
break;
case 3:
TCCR1B = 1<<CS12;//TCCR1B = 0x04; // Start TIMER1, Normal mode, crystal clock, prescaler = 256
break; 
case 4:
TCCR1B = 1<<CS12 | 1<<CS10;//TCCR1B = 0x05; // Start TIMER1, Normal mode, crystal clock, prescaler = 1024
break;
default:
TCCR1A = 1<<CS10;//TCCR1B = 0x01; Start TIMER1, Normal mode, crystal clock, no prescaler
break;
}

//while ((TIFR1 & (0x1<<TOV1)) == 0); // wait for TOV1 to roll over at 255 (requires preload of 65,535-5 to make count = 250)
// How does this while loop work?? See notes
while (TCNT1 < 0xfa); // exits when count = 250 (requires preload of 0 to make count = 250)

TCCR1B = 0x00; // Stop TIMER1
//TIFR1 = 0x1<<TOV1;  // Clear TOV1 (note that this is an odd bit in that it
//is cleared by writing a 1 to it)

} // end delay_T_msec_timer1()



