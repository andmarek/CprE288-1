/*
 * adc.c
 *
 *  Created on: Mar 5, 2019
 *      Author: jtbets12
 */

#include "adc.h"

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80



void init_ADC()
{
    // 1. Setup GPIO
     // A) Configure GPIO module associated with ADC
     // i. Turn on clock for GPIO Port B
     SYSCTL_RCGCADC_R |= 0b01;      //Causes freeze?
     SYSCTL_RCGCGPIO_R = 0x02;
     // ii. Enable alternate function
     GPIO_PORTB_AFSEL_R |= 0x10;

     // iii. Configure pins for input and analog mode
     GPIO_PORTB_DIR_R &= 0xEF;
     GPIO_PORTB_DEN_R &= 0xEF;
     GPIO_PORTB_AMSEL_R |= 0x10;
    // iv. Use default sample trigger source
    // By default, the processor (your program) is selected as the
    // trigger source and is initiated by the sample sequencer (SS)
     GPIO_PORTB_ADCCTL_R = 0x0; //Trigger in software
    // 2. Setup ADC
     // A) Configure ADC (choose ADC0 or ADC1)

         //ADC0_ADCCC = 0x0; //Use SysClk as ADC clk. Default, OK if omitted
     // Choose a sample sequencer (SS0, SS1, SS2 or SS3)
     // Disable ADC sample sequencer SS0 while configuring
     // OK to use SS1 or SS2 (cannot use SS3 for multiple samples)
     ADC0_ACTSS_R &= 0xFFFE;

     // Set ADC SS0 functionality
     ADC0_EMUX_R &= 0xFFF0;
     ADC0_SSMUX0_R |= 0x0000000A;
     // Configure which sample is end of sequence (bit 1 of 4-bit
     // field), after which sample the RIS flag is asserted (bit 2)
     // Use second sample (channel 1 is first, channel 9 is second)
     ADC0_SSCTL0_R = 0x00000000; // First clear register to 0
     ADC0_SSCTL0_R |= 0x00000002;


     // Re-enable ADC SS0
     ADC0_ACTSS_R |= 0x0001;
 }

int get_sensor_reading()
    {
    // Initiate SS0 conversion sequence
    ADC0_PSSI_R = 0b1;
    // Wait for SS0 ADC conversions to complete
    while((ADC0_PSSI_R & 0x80000000) == 0x80000000)
     {
     //wait
     }
    // Clear raw interrupt status flag for SS0
    ADC0_ISC_R = 0b1;
     // Get converted results from SS0 FIFO
     return ADC0_SSFIFO0_R;
   }

//Averages 180 values from a data set and returns the average value as a double
double average_sensor_reading(int data[])  {
    int IRaverages = 180;
    double average = 0.0;
    int i = 0;

    //Iterates through array to add 180 data points together
    while(i < IRaverages) {
        average += (double)data[i];
        i++;
    }

    //returns average of data set
    return average/IRaverages;
}
