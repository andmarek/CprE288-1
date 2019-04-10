#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

/**
 * main.c
 */
int main(void)
{
    init_portB();
    serial_init();

    while (1) {
        //Print each time the LED is turned ON or OFF
        if(flag == 1)
        {
            flag = 0; // Reset flag
            lcd_printf(“LED turned ON”);
        }
        if(flag == 2)
        {
            flag = 0; // Reset flag
            lcd_printf(“LED turned OFF”);
        }
        end if;
    }



	return 0;
}
