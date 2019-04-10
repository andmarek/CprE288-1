/*
 * movement.c
 *
 *  Created on: Jan 31, 2019
 *      Author: marcanio
 */
#include "movement.h"

/**
 * Moves forward a set amount of distance in CM
 */
void move_forward(oi_t *sensor, int centimeters){
    int sum =0;

    if(centimeters >0){
       oi_setWheels(250,250); // Start moving at full speed

        while(sum < centimeters * 10){
            oi_update(sensor);
            sum += sensor->distance;//Looks at the distance sensor and moves forward a set amount

        }
       oi_setWheels(0,0); //After moved distance STOP
    }
}
/**
 * Turns the wheels clockwise a certain amount of degrees
 */
void turn_right(oi_t *sensor, int degrees){
    double sum =0;

    if(degrees >0){
        oi_setWheels(-100, 100); // Turn Right

        while(sum < degrees){
            oi_update(sensor);

            sum = sum + -(sensor->angle) ; // Double This is used to take in account error
        }
        oi_setWheels(0, 0);// Reset wheels back  to 0,0 to start going straight again
    }


}
//Work on this
void turn_left(oi_t *sensor, int degrees){
    double sum =0;

       if(degrees >0){
           oi_setWheels(100, -100); // Turn Right

           while(sum < degrees){
               oi_update(sensor);

               sum +=  (double) sensor->angle ; // Double This is used to take in account error
           }
           oi_setWheels(0, 0);// Reset wheels back  to 0,0 to start going straight again
       }


}
/**
 * This is used to back up 15CM
 */
void back_up(oi_t* sensor){
          oi_setWheels(-300, -300);
          int backup =0;
                while( backup < 150){//Backs up for a total of 15 cm
                     oi_update(sensor);
                     backup -= sensor-> distance;
                  }
}
/**
 * Part 3 of the lab
 */
void collison_test(oi_t* sensor, int dis){
    int sum =0;
    int total = dis;
    int backcnt = 0;

    if(dis > 0){

        while(sum < dis*10){
            oi_setWheels(300, 300);
            oi_update(sensor);
            sum += sensor -> distance;
            if(sensor -> bumpLeft == 1 && sensor -> bumpRight ==0  ||sensor -> bumpLeft == 1 && sensor -> bumpRight ==1){// If left or both are bumped
                back_up(sensor);
                turn_right(sensor, 85);
                move_forward(sensor, 25);
                turn_left(sensor, 85);
                sum -= 150;// Since we backed up 15 cm subtract from total
            }
            if( sensor -> bumpLeft == 0 && sensor -> bumpRight ==1){//If right is bumped
                back_up(sensor);
                turn_left(sensor, 85);
                move_forward(sensor, 25);
                turn_right(sensor, 85);
                sum -= 150;// Since we backed up 15 cm subtract from total
            }


        }
        oi_setWheels(0,0);
    }
}


/**
 *

{

    Distance = Distance -20;

    int sum = 0;

    int travel = Distance;

    int c = 0;

    if (Distance > 0)

    {

        oi_setWheels(350, 350); // move forward;

        while (sum < Distance * 10)

        {

            oi_update(sensor);

            sum += sensor->distance;

            if (sensor->bumpLeft == 1 && sensor->bumpRight == 0

                    || sensor->bumpLeft == 0 && sensor->bumpRight == 1

                    || sensor->bumpLeft == 1 && sensor->bumpRight == 1)

            {

                Distance =0;

                int Sum = 0;



                oi_setWheels(-350, -350); // move backward;

                while (Sum < 150)

                {

                    oi_update(sensor);

                    Sum -= sensor->distance;



                }

                sum -= Sum;

                c = travel-(sum/10);

                turn_clockwise(sensor, 90);

                move_forward(sensor, 25);

                turn_clockwise(sensor, -90);

                move_forward(sensor, c);

                oi_setWheels(0,0);

            }

        }

        oi_setWheels(0, 0);

    }
 */



