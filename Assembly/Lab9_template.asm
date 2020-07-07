/*
 * Lab9_template.asm
 *
 *  Created: 7/7/2020
 *   Author: wclark
 */ 
;=============================================
; INCLUDE FILES
; header file for target AVR type
 .NOLIST                   ;Don't list the following in the list file
 .INCLUDE "m328pdef.inc"   ;ATmega328p
 .LIST                     ;Switch list on again
;
; ============================================
; CONSTANTS
; [Add all constants here that can be subject
; to change by the user]
; Format: .EQU const = $ABCD
; ============================================
; SRAM PLACEMENT
.DSEG       ;Data segment
.ORG 0X100  ;Set data segment origin
; Format: Label: .BYTE N ; reserve N Bytes from Label:
; ============================================
; RESET AND INTERRUPT VECTORS
.CSEG          ;Code segment
.ORG 0x0000      ;Set program origin
	JMP Main  ; Reset vector, jump to main loop, 1.  Note that actual code is placed further down the program memory so as not to sit on top of vectors.
	JMP Int0_name ; INT0, 2 -- Interrupt Vector for INT0.  Points to INT0 code.
	JMP Int1_name   ;INT1, 3 -- Interrupt Vector for INT1.  Points to INT1 code.
/*	RETI ;PCINT0 not used,4
	RETI ;PCINT1 not used,5
	RETI ;PCINT2 not used,6
	RETI ;WDT not used,7
	RETI ;TIMER2 COMPA not used,8
	RETI ;TIMER2 COMPB not used,9
	RETI ;TIMER2 OVF not used,10
	RETI ;TIMER1 CAPT not used,11
	RETI ;TIMER1 COMPA not used,12
	RETI ;TIMER1 COMPB not used,13
	RETI ;TIMER1 OVF not used,14
	RETI ;TIMER0 COMPA not used,15
	RETI ;TIMER0 COMPB not used,16
	RETI ;TIMER0 OVF not used,17
	RETI ;SPI,STC not used,18
	RETI ;USART,RX not used,19
	RETI ;USART,UDRE not used,20
	RETI ;USART,TX not used,21
	RETI ;ADC not used,22
	RETI ;EE READY not used,23
	RETI ;ANALOG COMP not used,24
	RETI ;TWI not used,25
	RETI ;SPM READY not used,26*/
; ============================================
; INTERRUPT SERVICE ROUTINES
Int0_name:
	; Is there anything else that needs to be done at the beginning of the Interrupt routine?
	
	; Save important information to the Stack
	PUSH R16 // put R16 data onto the stack in case there is useful information in that register from Main program
	IN R16, PORTC // Save the state of the LEDs currently being displayed on PORTC
	PUSH R16 
	IN R16, TCNT0 // save the counter value onto the Stack
	PUSH, R16
	IN R16, SREG // save Status Register onto the Stack
	PUSH R16 
	; Is there any other information that should be preserved? Note that the program counter is automatically pushed to the stack.

	; Create the required LED sequence for the Interrupt 0 function

	; Clean up before exiting ISR:
	; Generally you should undo everything that you did when you entered the ISR. 
	; When popping data off the Stack, be sure to do it in reverse order because the Stack operates as a LIFO (last in first out) buffer.
	; Some example clean up tasks:
	POP R16 ; pull Status Register off the Stack and restore it (make sure that there is nothing else on top of it on the Stack above this)
	OUT SREG, R16 
	; Is there anything else on the Stack to be restored?
	; Is there anything else to be returned to its proper state?
	; Anything else to be done befoere exiting the ISR?

	RETI // Return from interrupt.  Program Counter is reloaded with instruction address in main code where it left off.

Int1_name:
	; Interrupt 1 should be similar to Interrupt 0, but with different LED sequence

	RETI
;
; ============================================
; MAIN PROGRAM -- SETUP
Main:
	/* Initialize stack */
	LDI R16, HIGH(RAMEND) ; Init HSB stack
	OUT SPH,R16
	LDI R16, LOW(RAMEND)  ; Init LSB stack
	OUT SPL,R16           ; to stack pointer

	/* Enable External Interrupts */
	/* EIMSK: external interrupt mask register */
	LDI R16, (1<<INT0)|(1<<INT1)  ;enable EXINT 0 and EXINT 1
	OUT EIMSK, R16

	/* Select type of interrupt */
	/* EICRA: external interrupt control register */
	; Be sure that the interrupt type is properly set for both interrupts
	LDI R16, (1<<ISC01)|(1<<ISC00)|(1<<ISC10)|(1<<ISC11) ;EXINT0: rising edge, EXINT1: rising edge
	/* Attention: cannot use OUT to load value into EICRA, must use STS */
	STS EICRA, R16
	 
	/*********/
	; Initialize ports and do other housekeeping before you main code starts.
	/*********/
	LDI R16, 0b11111111 ;Set all PORTC bits as output (though we only need to use PC0-PC3)
	OUT DDRC, R16

	SEI // Set Global Interrupt Enable.  Interrupts can occur after this point.
;
; ============================================
; MAIN PROGRAM -- LOOP
Loop_main: 
	; Create the scrolling LED sequence in the Main loop
	LDI R16, 0b11111110
	OUT PORTC, R16
	CALL wait
	; Is the delay correct?
	LDI R16, 0b11111101
	OUT PORTC, R16
	; finish the sequence...
RJMP Loop_main ; infinite loop 

; ============================================
; SUBROUTINES
wait: // this subroutine creates a delay of 0-255 msec by repeatedly calling delay_1msec
	LDI R22, 250 // choose the number of msec to delay
	count_down:
	CALL delay_1msec
	DEC R22
	BRNE count_down
	RET // return from subroutine

delay_1msec: // this subroutine creates a 1msec delay with TIMER0 register and prescaler of 64 (assumes 16MHz clock)
	LDI R20, 0x00 // clears WGM00 and WGM01 (bits 0 and 1) to ensure Timer/Counter is in normal mode.
	OUT TCCR0A, R20
	
	LDI R20, 0x00 // clear timer register
	OUT TCNT0, R20
	
	LDI R21, 250 // set desired count value for later compare
		
	LDI R20, 0b00000011 //1<<CS01 | 1<<CS00;	TCCR0B = 0x03;  // Start TIMER0, Normal mode, crystal clock, prescaler = 64
	OUT TCCR0B, R20

	keep_counting:
	IN R20, TCNT0  //read TCNT0 and compare to desired count.  Loop until count = 250.
	CPSE R20, R21
	RJMP keep_counting

	LDI R20, 0x00; // Stop TIMER0
	OUT TCCR0B, R20
	RET // return from subroutine




