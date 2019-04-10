#include "lcd.h"

/**
 * main.c
 *
 * Simple 'Hello, world program'.
 * This program prints "Hello, world" to the LCD screen
 * @author 
 * @date 
 */
int main(void)
{
    //Initialize the LCD. This also clears the screen
    lcd_init();

    char array[33] = "YO";

	// Clear the LCD screen and print "Hello, world" on the LCD
	//lcd_puts("Hello, world");
	
	d_rotatingBanner(array);

	return 0;
}
