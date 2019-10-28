#include "lcd.h"
#include "uart.h"
#include "lab5_scan_data.h"
#include "adc.h"
#include "math.h"
#include "servo.h"
#include "stdbool.h"
#include "button.h"
#include "cyBot_Sonar.h"
#include "movement.h"
#include "open_interface.h"
#include "scan.h"
#include "song.h"

int cliffVal = 0;
char input = ' ';
char sendStr[120];

int main(void) {


    servo_init();
    init_ADC();
    TIMER3B_init();
    uart_init();
    lcd_init();
    oi_t *sensor_data = oi_alloc();
//    oi_init(sensor_data);

    while(1){
        input = uart_receive();
//        char toSend[120];
//        sprintf(toSend, "%c\r\n", input); //prints character entered
//        uart_sendStr(toSend);

        oi_init(sensor_data);
        oi_update(sensor_data);

        //Used for testing cliff sensor values
        //lcd_printf("Left: %d\nFront Left: %d\nFront Right: %d\nRight: %d\n", sensor_data -> cliffLeftSignal, sensor_data -> cliffFrontLeftSignal, sensor_data -> cliffFrontRightSignal, sensor_data -> cliffRightSignal);
        //lcd_printf("Battery: %d", sensor_data -> batteryCharge);
        //move forwards with cliff detection
        if (input == 'w')
            {
                cliffVal = move_forward_cliff(sensor_data, 300);

                char moveForward[] = "moving forward 30cm...\r\n";
                uart_sendStr(moveForward);
                if(cliffVal == 4)   {
                    oi_init(sensor_data);
                }   else if (cliffVal == 0)  {
                    uart_sendStr("left cliff sensor triggered\r\n");
                }   else if (cliffVal == 1)   {
                    uart_sendStr("left front cliff sensor triggered\r\n");
                }   else if (cliffVal == 2) {
                    uart_sendStr("right front cliff sensor triggered\r\n");
                }   else if (cliffVal == 3) {
                    uart_sendStr("right cliff sensor triggered\r\n");
                }   else if (cliffVal == 5) {
                    uart_sendStr("right bump sensor triggered\r\n");
                }   else if (cliffVal == 6) {
                    uart_sendStr("left bump sensor triggered\r\n");
                }   //else if (cliffVal == 7)  {
//                    uart_sendStr("left cliff sensor triggered, pit detected\r\n");
//                }   else if (cliffVal == 8)   {
//                    uart_sendStr("left front cliff sensor triggered, pit detected\r\n");
//                }   else if (cliffVal == 9) {
//                    uart_sendStr("right front cliff sensor triggered, pit detected\r\n");
//                }   else if (cliffVal == 10) {
//                    uart_sendStr("right cliff sensor triggered, pit detected\r\n");
//                }

            }
        else if (input == 't')
                {
                    cliffVal = move_forward_cliff(sensor_data, 80);

                    char moveForward[] = "moving forward...\r\n";
                    uart_sendStr(moveForward);
                    if(cliffVal == 4)   {
                        oi_init(sensor_data);
                    }   else if (cliffVal == 0)  {
                        uart_sendStr("left cliff sensor triggered, turned clockwise 90 degrees\r\n");
                    }   else if (cliffVal == 1)   {
                        uart_sendStr("left front cliff sensor triggered, turned clockwise 90 degrees\r\n");
                    }   else if (cliffVal == 2) {
                        uart_sendStr("right front cliff sensor triggered, turned counterclockwise 90 degrees\r\n");
                    }   else if (cliffVal == 3) {
                        uart_sendStr("right cliff sensor triggered, turned counterclockwise 90 degrees\r\n");
                    }   else if (cliffVal == 5) {
                        uart_sendStr("right bump sensor triggered\r\n");
                    }   else if (cliffVal == 6) {
                        uart_sendStr("left bump sensor triggered\r\n");
                    }

                }
            //backwards
        else if (input == 's')
            {
                char moveBackwards[] = "moving backwards...\r\n";
                uart_sendStr(moveBackwards);
                move_backward(sensor_data, 300); //backward
            }

        else if (input == 'g')
            {
                char moveBackwards[] = "moving backwards...\r\n";
                uart_sendStr(moveBackwards);
                move_backward(sensor_data, 80); //backward
            }

            //rotates clockwise
        else if (input == 'd')
            {
                char turnRight[] = "rotating clockwise 45 degrees...\r\n";
                uart_sendStr(turnRight);
                turn_right(sensor_data, 38);//rotate right 45 degrees

            }

        else if (input == 'h')
            {
                char turnRight[] = "rotating clockwise 22.5 degrees...\r\n";
                uart_sendStr(turnRight);
                turn_right(sensor_data, 15);//rotate right 22.5 degrees

            }

            //rotates counterclockwise
        else if (input == 'a')
            {
                char turnLeft[] = "rotating counterclockwise 45 degrees...\r\n";
                uart_sendStr(turnLeft) ;
                turn_left(sensor_data, 38) ;//rotate 45 degrees left
            }

        else if (input == 'f')
            {
                char turnLeft[] = "rotating counterclockwise 22.5 degrees...\r\n";
                uart_sendStr(turnLeft) ;
                turn_left(sensor_data, 15) ;//rotate 22.5 degrees left
            }

        //sweep
        else if(input == 'k') {
           scan();
        }

        else if(input == ' ') {
            loadSong();
            playSong();
        }

        else if(input == 'p'){
            sprintf(sendStr,"%-20s%-20s\r\n", "Input", "Explanation");
            uart_sendStr(sendStr);
            sprintf(sendStr,"%-20s%-20s\r\n%-20s%-20s\r\n%-20s%-20s\r\n%-20s%-20s\r\n%-20s%-20s\r\n%-20s%-20s\r\n%-20s%-20s\r\n%-20s%-20s\r\n%-20s%-20s\r\n%-20s%-20s\r\n",
                    "w", "Will move the bot forward 30 centimeters or until it hits a boundary or object",
                    "s", "Will move the bot backwards 30 centimeters",
                    "d", "Will rotate the bot by 45 degrees to the right",
                    "a", "Will rotate the bot by 45 degrees to the left",
                    "k", "Will make the bot scan in front of itself by 180 degrees",
                    "t", "Will move the bot forward 10 centimeters or until it hits a boundary or object",
                    "g", "Will move the bot backwards 10 centimeters",
                    "h", "Will rotate the bot by 22.5 degrees to the right",
                    "f", "Will rotate the bot by 22.5 degrees to the left",
                    "space", "Will play a tune");

            uart_sendStr(sendStr);
        }

        oi_free(sensor_data);

    }
    return 0;
}
