/*
 * ping.h
 *
 *  Created on: Mar 12, 2019
 *      Author: marcanio
 */

#ifndef PING_H_
#define PING_H_

volatile enum {LOW, HIGH, DONE} state;
void ping_init();
void ping_read();
unsigned send_pulse();
void TIMER1A_HANDLER(void);
volatile int distance;
#endif /* PING_H_ */
