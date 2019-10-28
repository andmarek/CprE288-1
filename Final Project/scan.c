/*
 * servo.c
 *
 *  Created on: Apr 23, 2019
 *      Author: gmrueger
 */

#include "scan.h"
#include "uart.h"
#include "servo.h"
#include "timer.h"
#include "cyBot_Sonar.h"
#include <stdio.h>
#include "math.h"
#include "adc.h"

struct Vals {
    double IR;
    double ping;
};

struct ObjDet   {
    double center;
    double distance;
    double radial;
    double linear;
};

const int IRaverages = 180;
int i;
int k;
int l;
int m;
char sendstr[120];
int dataOne[IRaverages];
struct Vals sweep[91];
struct ObjDet obstacle[20];
int totalObjs;
int flag;
int startIndex;
int endIndex;
double frontEdgeIR;
int smallestIndex;


void scan(void)   {
    i = 0;
    k = 0;
    l = 0;
    m = 0;
    memset(sendstr, 0, sizeof(sendstr));
    memset(dataOne, 0, sizeof(dataOne));
    memset(sweep, 0, sizeof(sweep));
    memset(obstacle, 0, sizeof(obstacle));
    totalObjs = 0;
    flag = 0;
    smallestIndex = 0;


//sprintf(sendstr, "%-20s%-20s%-20s\r\n", "Degrees", "IR Distance (cm)", "Sonar Distance(cm)");
    uart_sendStr("scanning for objects...\r\n");

//ensures servo can point at 0 before measurements are taken

    servo_move(0);
    timer_waitMillis(500);

    while(i <= 90)   {
        servo_move(i * 2);


        int j = 0;

        //IR reading
        //Averages 16 sets of data to give a more precise sensor reading
        while(j < IRaverages)    {
            //Takes 16 data points
            dataOne[j] = get_sensor_reading();
            j++;
        }

        //averages the 16 data sets
        sweep[i].IR = average_sensor_reading(dataOne);


        //Calculates distance based on sensor data
        //IR = (7000000*pow(IR,-1.721));     //Equation for CyBot 0
        sweep[i].IR = 4834964.42*pow(sweep[i].IR, -1.63);
                                //(6.757187+((50.45176-6.757178)/pow(1+pow((sweep[i].IR/1203.806),706.2403),.003406494)));   //Equation for Cybot 10
        //End IR reading

        sweep[i].ping = ping_read();

        //sprintf(sendstr, "%-20d%-20.1lf%-20.1lf\r\n", i*2, sweep[i].IR, sweep[i].ping);


        //lcd_printf("IR: %.1lf\r\n Ping: %.1lf", sweep[i].IR, sweep[i].ping);
        //uart_sendStr(sendstr);
        i++;
        timer_waitMillis(75);
    }

    while(k <= 90)      {
        if(flag == 0)   {
            if((sweep[k].ping < 80) && (sweep[k].IR < 45))    {
                startIndex = k;
                frontEdgeIR = sweep[k].IR;

                flag = 1;
            }
        }   else if(flag == 1)  {
            if((sweep[k].ping > 80) || (sweep[k].IR > 45))  {
                endIndex = k;
                obstacle[totalObjs].radial = 2.0*(endIndex - 1.0) - 2.0*(startIndex - 1.0);
                obstacle[totalObjs].center = 2*(startIndex) + obstacle[totalObjs].radial/2;
                obstacle[totalObjs].distance = sweep[(int)(obstacle[totalObjs].center/2)].ping; //Found correctly
                obstacle[totalObjs].linear = 2*(obstacle[totalObjs].distance*tan((obstacle[totalObjs].radial/2)*(M_PI/180)));

                flag = 2;
                totalObjs++;
            }
        }   else if(flag == 2)  {
            if(sweep[k].IR > 80)    {
                flag = 0;
            }
        }
        k++;

    }

    sprintf(sendstr, "%-20s%-20s%-20s%-20s\r\n", "Center Angle", "Distance (cm)", "Width (degrees)", "Linear Width (cm)");
    uart_sendStr(sendstr);


    while(l < totalObjs)    {
        sprintf(sendstr, "%-20lf%-20.1lf%-20.1lf%-20lf\r\n", obstacle[l].center, obstacle[l].distance, obstacle[l].radial, obstacle[l].linear);
        uart_sendStr(sendstr);
        l++;
    }



    for(m = 0; m < totalObjs; m++)  {
        if(obstacle[m].linear < obstacle[smallestIndex].linear)    {
            smallestIndex = m;
        }
    }

    lcd_printf("Index: %d\nDistance:%.2lf\nLocation: %.2lf\nWidth: %.2lf", smallestIndex + 1, obstacle[smallestIndex].distance, obstacle[smallestIndex].center, obstacle[smallestIndex].linear);
    servo_move(obstacle[smallestIndex].center);

}
