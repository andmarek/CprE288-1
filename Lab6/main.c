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
#include <math.h>
#include "timer.h"
volatile int flag = 0; // flag updated by ISR
volatile char str[21];
char sendstr[100];



int main() {

    lcd_init();
    init_ADC();
    serial_init();
    double estimated =0;
    while(1){
        int temp = (adc_read());
       // estimated = 1.25996 + (48.00231 - 1.25996)/ pow((1 + pow((temp/ 677.9077),357.1603)), 0.003293585);
       //estimated = 3.0191102+ (96.45314-3.019102) / (1+ pow((temp/259.4033), 1.380871));
        estimated = -1.745222 + (48.34164 + 1.745222) / pow((1 + pow((temp/743.249), 516.1554)), .002017931);
        sprintf(sendstr,  "%.5d %.5lf", temp, estimated);
        lcd_printf(sendstr); //"Q:%.4d, E: %.4lf", temp, estimated
        timer_waitMillis(1000);

        uart_senddata(sendstr);

        uart_sendChar('\r');
        uart_sendChar('\n');


    }

    /**
     * for(int i
     */

    return 0;
}
