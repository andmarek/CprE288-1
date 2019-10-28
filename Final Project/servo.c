/*
 * servo.c
 *
 *  Created on: Mar 26, 2019
 *      Author: gmrueger
 */

#include "servo.h"

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

const uint32_t zero = 0x4E200 - 0x48DA0;

void servo_init(void)   {
    //Set up GPIO
    //enable clock to GPIO, R1 = port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    //enable clock to Timer1B
    SYSCTL_RCGCTIMER_R |= 0b00000010;

    //Set digital or analog mode, and set pin directions
    GPIO_PORTB_DEN_R |= BIT5;  //Set pin 5 digital mode

    //Enable Alternate function for PB5, and set functionality
    GPIO_PORTB_AFSEL_R |= (BIT5);
    GPIO_PORTB_PCTL_R |= 0x00700000;    //Use bit 7 for PB5

    //set to output
    GPIO_PORTB_DIR_R |= BIT5;   //Probably don't need as input

    //Set up Timer1B       (see page 724 of datasheet)
    //Disable Timer
    TIMER1_CTL_R &= 0xFFFFFEFF;
    TIMER1_CFG_R = 0x00000004;
    TIMER1_TBMR_R &= 0xEF;          //Set to down count
    TIMER1_TBMR_R |= 0x0000000A;    //TBAMS to 0x1 TBCMR to 0x0 TBMR to 0x2
    //TIMER1_CTL_R |= 0x00000C00;   //Do we need timer event mode?
    //TIMER1_CTL_R |= 0x4000;       //Inverts output to servo

    //Set 8-bit prescaler
    TIMER1_TBPR_R = 0x4;


    //Set max value
    TIMER1_TBILR_R = 0xE200;       //320000 = 0x4E200
    //Set match value
    TIMER1_TBPMR_R = 0x4;
    TIMER1_TBMATCHR_R = 0x8680;     //304000 = 0x48DA0  //C580 = zero for bot 3, 180 degrees at 0x5700
                                    //bot 7: C580 ~= 0 degrees 5900 ~= 180 degrees
                                    //bot 2: C200 ~= 0 degrees 5600 ~= 180 degrees
                                    //bot 9: C800 ~= 0 degrees 5800 ~= 180 degrees
                                    //bot 11: BD00 ~= 0 degrees 5000 ~= 180 degrees

    //timer_waitMillis(20);


    //Clear event interrupt
    TIMER1_ICR_R |= 0x400;
//    //Turn on interrupts
//    TIMER1_IMR_R |= 0x400;
//
//    //NVIC
//    NVIC_PRI9_R |= 0x20;
//    NVIC_EN1_R |= 0x00000010;
//
//    //Bind Timer 3B (52) interrupt requests to user's interrupt handler
//    IntRegister(52, ping_Handler);
//
    //Enable counter and start counting
    TIMER1_CTL_R |= 0x00000100;

//
//    IntMasterEnable();
}

int servo_move(double degrees)   {
    double ILRval = ((-155.0222 * degrees) + 310528 - 262144);    //For bot 7: ((-154.3111 * degrees) + 312704.0 - 262144)
                                                                //For bot 2: ((-153.6 * degrees) + 311808 - 262144)
                                                                  //For bot 9 : ((-159.2889 * degrees) + 313344 - 262144)
                                                                //For bot 11: ((-155.0222 * degrees) + 310528 - 262144)
    uint32_t position = (uint32_t) ILRval;
    TIMER1_TBMATCHR_R = position;
    return position;
}

