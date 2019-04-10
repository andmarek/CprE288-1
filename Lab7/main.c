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



extern volatile int data;
extern volatile int OF;




int main(void) {

    lcd_init();
    serial_init();
    ping_init();



    while(1){
       send_pulse();
       ping_read();
       lcd_printf("time is: %d\noverflow: %d", data, OF);// Part 2
       timer_waitMillis(10);
    }




    return 0;
}
