/**
 * main.c
 * @author: marcanio, ziska
 * @date:
 */

#include "lcd.h"
#include "button.h"

volatile int button_event = 0; // Boolean to keep track of whether a hardware event has happened (button pressed)
volatile int button_num = 0; // keeps track of button pressed

/**
 * main.c
 * Prints the button that has been pressed
 */
int main(void) {
   // #warning "Unimplemented function: int main(void)" // delete warning after implementing
    // Hint: You may need to use pointers to return the button that has been pressed
    button_init();
    lcd_init();

    init_button_interrupts(&button_event, &button_num);
    while(1){

        uint8_t button = button_getButton();


       // gpioe_handler();
        if(button_event == 1){
            button_event = 0;
             lcd_printf("Button: %d", button_num);
        }

     // lcd_printf("button: %d", button_num);
    }



}
