
// This program is a template for controlling a stepper motor.
// The code is not complete, so will not stand alone, but 
// functions are provided with which to step a motor in forward
// or backward directions.

// Functions
void step_CW( void );
void step_CCW( void );

//** GLOBAL VARIABLES **
int phase_step = 1; // We will use wave stepping, so the steps are numbered 1-4, then will repeat.  

int main(void)
{
   // Setup
	DDRD = 0b11110000; // Sets pins 4, 5, 6, and 7 output, and 	pins 0, 1, 2 and 3 to input. Pins 4-7 will be used for 	stepper motor control
   while(1)
   {
	// Do something that needs to step forward
	step_CW(); //step_CW()
	wait(250,2); // delay so that motor doesn't skip steps

	// Do something that needs to step backward
	step_CCW(); // step_CCW()
	wait(250,2); // delay so that motor doesn't skip steps
  
   } // end main while

}

void step_CW() {
// This function advances the motor clockwise one step.  Follow the full-wave stepping table in Stepper Motor Control.ppt for MEMS 1049 course. Our stepper motor phases are assigned to Port pins as follows: phase1a = PORTD_7, phase1b = PORTD_6, phase2a = PORTD_5, phase2b = PORTD_4

  switch (phase_step) {
    case 1:
      // step to 4
      PORTD = 0b00100000;
      phase_step = 4;
	break;
    case 2:
      // step to 1
      PORTD = 0b10000000;
      phase_step = 1;
	break;
     case 3: 
      // step to 2;
      PORTD = 0b00010000;
      phase_step = 2;
	break;
     case 4:
     // step to 3;
      PORTD = 0b01000000;
      phase_step = 3;
	break;
  } 
}// end step_CW
void step_CCW()
{
// This function advances the motor counter-clockwise one step.  Follow the full-wave stepping table in Stepper Motor Control.ppt for MEMS 1049 course. phase1a = PORTD_7, phase1b = PORTD_6
// phase2a = PORTD_5, phase2b = PORTD_4

  switch (phase_step) {
    case 1:
      // step to 2
      PORTD = 0b00010000;
      phase_step = 2;
	break;
    case 2:
      // step to 3
      PORTD = 0b01000000;
      phase_step = 3;
	break;
     case 3: 
      // step to 4;
      PORTD = 0b00100000;
      phase_step = 4;
	break;
      case 4:
     // step to 1;
      PORTD = 0b10000000;
      phase_step = 1;
	break;
  }
}// end step_CCW
