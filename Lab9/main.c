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
#include <math.h>
#define PI 3.14159265358979323846

extern volatile int data;
extern volatile int OF;
volatile int distance;
char sendstr[100];

typedef struct values{
    int angle;
    double IR;
    double sonar;
}values;

 int init_angle[10];
 int final_angle[10];
 int widths[10];
 values aarray[90];
 double IR =0;

int main(void) {
    //Initializing the functions needed
    servo_init();
    lcd_init();
    serial_init();
    ping_init();
    init_ADC();

    int i =0;//Current Angle
    int smallest =100;//Smallest object
    int items = 0;// Amount of items
    int obj =0;// Boolean used to detect if on an object or not
    int oa= 1; //Used to count angles
    int angle =0;


    for(i =0; i <= 180; i +=2 ){
        angle =i; // Current angle
        servo_move(angle); // Spanning the sensors

        send_pulse();// Used for sonar
        ping_read();

        int ping = (adc_read());
        IR = -1.745222 + (48.34164 + 1.745222) / pow((1 + pow((ping/743.249), 516.1554)), .002017931);// Shift IR-- BOT 10

        aarray[angle].angle = angle;//Put everything inside struct
        aarray[angle].IR = IR;
        aarray[angle].sonar = distance;

        if(IR < 45.0 && obj == 0){ //Currently on an object
            obj =1;
            init_angle[oa] = i;
        }
        else if(IR> 45.0 && obj== 1){//Finished looking at object
           final_angle[oa] = i;
           widths[oa] = sqrt(pow(aarray[init_angle[oa]].sonar, 2) + pow(aarray[final_angle[oa]].sonar, 2) - (2 * aarray[init_angle[oa]].sonar * aarray[final_angle[oa]].sonar * cos(final_angle[oa] - init_angle[oa])));

           if(widths[oa] < smallest && widths[oa] != 0){//Check if the width of the object is the smallest
            smallest = oa;
           }
            oa++;
            obj =0;
            items++;
        }

        timer_waitMillis(100);

       lcd_printf("Angle: %d \n IR: %lf \n Objects: %d ", aarray[angle].angle, aarray[angle].IR, oa-1);

        //sprintf(sendstr, "Ping: %d IR: %.5lf\r\n",distance, IR);
        //uart_sendstr(sendstr);// Sends to uart
    }
    int katie = (init_angle[smallest] + final_angle[smallest])/ 2;// Point to middle of smallest object
    servo_move(katie);
    lcd_printf("Smallest: \n Angle %d to %d", init_angle[smallest], final_angle[smallest] );
    timer_waitMillis(2000);
    int ka =0;
    for(ka =0; ka<= oa; ka++){
        lcd_printf("ka : Angle %d to %d", init_angle[ka], final_angle[ka] );
        timer_waitMillis(1000);
    }
    return 0;
}
