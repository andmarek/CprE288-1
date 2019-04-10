/*
 * servo.c
 *
 *  Created on: Mar 26, 2019
 *      Author: marcanio
 */

#include "lcd.h"
#include "uart.h"
#include "button.h"
#include "lab5_scan_data.h"
#include "adc.h"
#include "timer.h"
#include "ping.h"
#include "servo.h"





int servo_move(double degrees){

    //calculate the number the timer needs to match

        int result = 320000 - (7112 + (153.08333 * degrees));

        TIMER1_TBMATCHR_R = result & 0xFFFF;

        TIMER1_TBPMR_R = (result >> 16) & 0xFF;

        return result;
}


void servo_init(){

    SYSCTL_RCGCGPIO_R |= 0x2;     //enable Clock on PortB
    GPIO_PORTB_AFSEL_R |= 0x20;   //Set pin 5 to alt function
    GPIO_PORTB_PCTL_R |= 0x700000;//enable GPIO pin 5
    GPIO_PORTB_DIR_R |= 0x20;     //Enable pin 5 output
    GPIO_PORTB_DEN_R |= 0x20;     //pin 5 to digital


    SYSCTL_RCGCTIMER_R |=SYSCTL_RCGCTIMER_R1; // Start clock
    //Steps on page 725
    TIMER1_CTL_R &= 0xFEFF; // Disable the timer
    TIMER1_CFG_R |= TIMER_CFG_16_BIT;    // Configuration register- 16 bit
    TIMER1_TBMR_R |= 0x0000000A; // Sets to periodic mode
    TIMER1_TBPR_R = (320000 >> 16) & 0xFF;  //overflow for where the timer starts
    TIMER1_TBILR_R = 320000 & 0xFFFF;   //first 16 bits for the timer
    TIMER1_TBPMR_R = 0x00;
    TIMER1_TBMATCHR_R = 0x3E80; //value the timer needs to match before doing an event
    TIMER1_CTL_R |= 0x0100; //enable the timer

    //TIMER1_TBPMR_R = 0x00;  // most sig bits for the value MATCHR NOT NEEDED interupts?

    return;
}
