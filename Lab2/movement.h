/*
 * movement.h
 *
 *  Created on: Jan 31, 2019
 *      Author: marcanio
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include "open_interface.h"
//#include "movement.c"

void move_forward(oi_t *sensor, int centimeters);
void turn_right(oi_t *sensor, int degrees);
void turn_left(oi_t *sensor, int degrees);



#endif /* MOVEMENT_H_ */
