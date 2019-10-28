/*
 * servo.h
 *
 *  Created on: Mar 26, 2019
 *      Author: gmrueger
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

//initializes the servo motor and timer 1B
void servo_init(void);

//moves the servo motor to the specified position in degrees
int servo_move(double degrees);



#endif /* SERVO_H_ */
