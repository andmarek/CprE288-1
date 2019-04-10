/**
 * main.c
 * @author: marcanio, ziska
 * @date:
 */

#include "lcd.h"
#include "uart.h"
#include "button.h"
#include "lab5_scan_data.h"
#include "adc.h"
#include "timer.h"
#include "ping.h"
#include "servo.h"


extern volatile int data;
extern volatile int OF;


//Lab 8

int main(void) {
      //Initializing the functions needed
    servo_init();
    button_init();
    lcd_init();
    serial_init();
    ping_init();

    int result=0;
    int direction=1;
    double angle =0; //The number being calculted and sent to servo_move
    while (1)
    {
        uint8_t button = button_getButton(); //Action needed to take
        if (button != 0)// IF a button is pressed
        {
            if ((direction == 1 && angle < 180) // Make sure it doesnt go past 0 or 180
                    || (angle > 0 && direction == -1))
            {
                if (button == 1)
                {
                    angle += direction; // Moves 1 degree
                }
                else if (button == 2)
                {
                    angle += direction * 2.5; // Move 2.5 degrees
                }
                else if (button == 3)
                {
                    angle += direction * 5.0; // Move 5 degrees
                }
            }
            if (button == 4)
            {
                direction = -1 * direction; // Flip the direction
            }
            if(button == 5){ // Go to 0 degrees
                angle = 0;
            }
            if(button == 6){//Go to 180 degrees
                angle = 180;
            }
        }
        result = servo_move(angle);
        timer_waitMillis(100);// Need this so it wont keep going when you hold the button
        lcd_printf("angle: %.1lf\n raw: %d\n direction: %d", angle, result, direction);
    }




    return 0;
}
