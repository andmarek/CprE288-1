/*
 * servo.h
 *
 *  Created on: Mar 26, 2019
 *      Author: gmrueger
 */

#ifndef SCAN_H_
#define SCAN_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

//scans for objects
void scan(void);

#endif
