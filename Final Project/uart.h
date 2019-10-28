/*
 * uart.h
 *
 *  Created on: Feb 19, 2019
 *      Author: gmrueger
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"


//Initializes the given port of UART
void uart_init(void);

//The interrupt handler
void uart_Handler(void);

//Sends a character to putty
void uart_sendChar(char data);

//Displays given character from microcontroller
char uart_receive(void);

//Displays given string in putty
void uart_sendStr(const char *data);

#endif /* UART_H_ */
