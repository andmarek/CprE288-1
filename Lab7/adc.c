/*
 * adc.c
 *
 *  Created on: Mar 5, 2019
 *      Author: marcanio
 */

#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "uart.h"


void init_ADC(){

    //enable ADC 0 module on port D
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;
    //enable clock for ADC
    SYSCTL_RCGCADC_R |= 0x1;
    //enable port D pin 0 to work as alternate functions
    GPIO_PORTD_AFSEL_R |= 0x01;
    //set pin to input - 0
    GPIO_PORTD_DEN_R &= 0b11111110;
    //disable analog isolation for the pin
    GPIO_PORTD_AMSEL_R |= 0x01;
    //initialize the port trigger source as processor (default)
    GPIO_PORTD_ADCCTL_R = 0x00;
    //disable SS0 sample sequencer to configure it
    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;
    //average
    ADC0_SAC_R | 0x4;
    //initialize the ADC trigger source as processor (default)
    ADC0_EMUX_R = ADC_EMUX_EM0_PROCESSOR;
    //set 1st sample to use the AIN10 ADC pin
    ADC0_SSMUX0_R |= 0x000A;
    //enable raw interrupt status
    ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0 | ADC_SSCTL0_END0);
    //enable oversampling to average
    ADC0_SAC_R |= ADC_SAC_AVG_64X;
    //re-enable ADC0 SS0
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;

}

int adc_read(void){

    ADC0_PSSI_R = ADC_PSSI_SS0;//initiate SS1

    while((ADC0_RIS_R & ADC_RIS_INR0)== 0){
        //Wait till conversion is complete
    }

    int value = ADC0_SSFIFO0_R;

    return value;
}









//void init_ADC()
//{
//
// // i. Turn on clock for GPIO Port B
// SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R | 0b000010; // 0x10
// SYSCTL_RCGCADC_R |= 0x1; // Enable ADC0 clock
// // ii. Enable alternate function
// GPIO_PORTB_AFSEL_R |= 0x10; //Port B pin 4
//
//
//
// // iii. Configure pins for input and analog mode
//  GPIO_PORTB_DEN_R &= 0b11101111; //Disable digital mode pins 4
//
//  GPIO_PORTB_AMSEL_R |= 0x10; //Enable analog mode pins 4
// // GPIO_PORTB_DIR_R &= 0b11101111; //Pin 4 set to input
//
//
//
////Port trigger as proccesorr
// GPIO_PORTB_ADCCTL_R = 0x00; //Trigger in software
//// 2. Setup ADC
//
//
// //ADC0_ADCCC = 0x00; //Use SysClk as ADC clk. Default, OK if omitted
//
// // Disable ADC sample sequencer SS1 while configuring
// ADC0_ACTSS_R &= ~(0x00000002); //Disable ADC0 SS1 by clearing bit 0
//
// // Set ADC SS1 functionality
// ADC0_EMUX_R &= ~(0x000000F0); //Set SS1 to trigger using ADCPSSI reg.
// ADC0_SSMUX1_R |= 0x000A; // set first sample to use AIN10 ADC pin
//
// // Use second sample (channel 1 is first, channel 9 is second)
// ADC0_SSCTL1_R = 0x00000000; // First clear register to 0
// ADC0_SSCTL1_R |= 0x00000060; // Set bits 1 and 2 for second sample
//
// ADC0_SAC_R |= 0x4;
//
// ADC0_ACTSS_R |= 0x00000002;//Re-enable
// // Re-enable ADC SS0
//
//}
