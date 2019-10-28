/*
 *  button.c
 *  @author: Eric Middleton, Zhao Zhang, Chad Nelson, & Zachary Glanz.
 *  @date: Jul 18, 2016
 *
 *  @edit: Lindsey Sleeth and Sam Stifter on 02/04/2019
 */

#include "button.h"

// The buttons are on PORTE 0:5
// GPIO_PORTE_DATA_R -- the register that must be checked on port E to determine which buttons are being pressed

#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80

volatile int button_num = 0;
volatile int button_event = 0;

uint8_t prev_Button; //must be set yourself in button_getButton()
uint8_t button; //current button being pressed, must be set yourself in button_getButton()

// These variables are declared in button.h for your use in the interrupt handler.
// extern volatile int button_event = 0; // Boolean to keep track of whether a hardware event has happened (button pressed)
// extern volatile int button_num = 0; // keeps track of button pressed

/**
 * Initialize PORTE and configure bits 0-5 to be used as inputs for the buttons.
 */
void button_init() {
	static uint8_t initialized = 0;

	//Check if already initialized
	if(initialized){
		return;
	}

	// To initialize and configure GPIO PORTE, visit pg. 656 in the Tiva datasheet.
	// Follow steps in 10.3 for initialization and configuration. Some steps have been outlined below.
	// Ignore all other steps in initialization and configuration that are not listed below. You will learn more
	// about additional steps in a later lab.

	//Turn on PORTE system clock


	SYSCTL_RCGCGPIO_R |= 0b010000;

	//Set the buttons to inputs and enable
	GPIO_PORTE_DIR_R &= 0b00000000;
	GPIO_PORTE_DEN_R |= 0b11111111;

	initialized = 1;
}

/**
 * Interrupt handler -- executes when a hardware event occurs (a button is pressed)
 */
void init_button_interrupts() {

    // In order to configure GPIO ports to detect interrupts, you will need to visit pg. 656 in the Tiva datasheet.
    // Notice that you already followed some steps in 10.3 for initialization and configuration of the GPIO ports in the function button_init().
    // Additional steps for setting up the GPIO port to detect interrupts have been outlined below.

    // TODO: Complete code below

    button_init();

    // Mask the bits for pins 0-5
    GPIO_PORTE_IM_R &= 0b11000000;

    // Set pins 0-5 to use edge sensing
    GPIO_PORTE_IS_R &= 0b11000000;

    // Set pins 0-5 to use both edges. We want to update the LCD
    // when a button is pressed, and when the button is released.
    GPIO_PORTE_IBE_R |= 0b00111111;

    // Clear the interrupts
    GPIO_PORTE_ICR_R |= 0b00111111;

    // Unmask the bits for pins 0-5
    GPIO_PORTE_IM_R |= 0b00111111;

    // TODO: Complete code below
    // Enable GPIO port E interrupt
    NVIC_EN0_R |= 0x00000010;

    // Bind the interrupt to the handler.
    IntRegister(INT_GPIOE, gpioe_handler);
}

/**
 * Handles a hardware interrupt that occurs from a button being pressed
 */
void gpioe_handler() {

    // Clear interrupt status register
    GPIO_PORTE_ICR_R |= 0b00111111;
    button_event = 1;
    button_num = button_getButton();
}

/**
 * returns a 6-bit bit field, representing the push buttons. A 1 means the corresponding button is pressed.
 */
uint8_t button_checkButtons()
{
    return (~GPIO_PORTE_DATA_R) & (BIT6 - 1); //Return the button status
}

/**
 * Returns the position of the leftmost button being pushed.
 * @return the position of the leftmost button being pushed. A 6 is the leftmost button, 1 is the rightmost button. Return 0 if no button is being pressed.
 */
uint8_t button_getButton() {

	//
	// DELETE ME - How bitmasking works
	// ----------------------------------------
	// In embedded programming, often we only care about one or a few bits in a piece of
	// data. There are several bitwise operators that we can apply to data in order
	// to "mask" the bits that we don't care about.
	//
	//	| = bitwise OR		& = bitwise AND		^ = bitwise XOR		~ = bitwise NOT
	//		  << x = shift left by x bits		 >> x = shift right by x bits
	//
	// Let's say we want to know if push button 3 (SW3) of GPIO_PORTE_DATA_R is
	// pushed.  Since push buttons are high (1) initially, and low (0) if pushed, PORTE should
	// look like:
	// GPIO_PORTE_DATA_R => 0b???? ?0?? if SW3 is pushed
	// GPIO_PORTE_DATA_R => 0b???? ?1?? if SW3 is not pushed
	//
	// Bitwise AND:
	// (GPIO_PORTE_DATA_R & 0b0000 0100) => 0b0000 0000 if SW3 is pushed
	// (GPIO_PORTE_DATA_R & 0b0000 0100) => 0b0000 0100 if SW3 is not pushed
	//
	// Bitwise OR:
	// (GPIO_PORTE_DATA_R | 0b1111 1011) => 0b1111 1011 if SW3 is pushed
	// (GPIO_PORTE_DATA_R | 0b1111 1011) => 0b1111 1111 if SW3 is not pushed
	//
	// Other techniques (Shifting and bitwise AND)
	// ((GPIO_PORTE_DATA_R >> 2) & 1) => 0 if SW3 is pushed
	// ((GPIO_PORTE_DATA_R >> 2) & 1) => 1 if SW3 is not pushed

    // TODO: Write code below -- Checks the leftmost bit, which corresponds to 6

    if((GPIO_PORTE_DATA_R | 0b11011111) == 0b11011111) {       //Checks in order from leftmost(6) button to rightmost (1), returns 0 if none pressed
       return 6;
    }
    if((GPIO_PORTE_DATA_R | 0b11101111) == 0b11101111) {
        return 5;
    }
    if((GPIO_PORTE_DATA_R | 0b11110111) == 0b11110111) {
        return 4;
    }
    if((GPIO_PORTE_DATA_R | 0b11111011) == 0b11111011) {
        return 3;
    }
    if((GPIO_PORTE_DATA_R | 0b11111101) == 0b11111101) {
        return 2;
    }
    if((GPIO_PORTE_DATA_R | 0b11111110) == 0b11111110) {
        return 1;
    }
    else    {
        return 0;
    }

}


