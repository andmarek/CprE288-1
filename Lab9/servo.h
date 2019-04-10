/*
 * servo.h
 *
 *  Created on: Mar 26, 2019
 *      Author: marcanio
 */

#ifndef SERVO_H_
#define SERVO_H_

volatile signed pulse_width;
void servo_init();
int servo_move(double degrees);



#endif /* SERVO_H_ */
