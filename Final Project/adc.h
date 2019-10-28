/*
 * adc.h
 *
 *  Created on: Mar 5, 2019
 *      Author: jtbets12
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

//Initializes the ADC for lab 6
void init_ADC();

//Gets input from the sensors
int get_sensor_reading();

//averages sensor reading using 16 points
double average_sensor_reading(int data[]);


#endif /* ADC_H_ */
