#include "lcd.h"
#include "open_interface.h"
#include "movement.h"
//#include "movement.c"


/**
 * Used to call move forward in 1 sweep
 */
void mforward(){
    oi_t *sensor_data = oi_alloc(); //Access the distance sensor

    oi_init(sensor_data);
    move_forward(sensor_data, 50); //Uses the distance sensor and asks how far
    oi_free(sensor_data);
}

/**
 * Sets up turn so i can call it in one function
 */
void turn(){

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    turn_right(sensor_data , 85);

    oi_free(sensor_data);
}

/**
 * Commands used to move robot in square
 */
void square(){
    mforward();
    turn();
    mforward();
    turn();
    mforward();
    turn();
    mforward();

}

/**
 * This program moves the robot 2 M and detects anything along the way
 */
void collison(){
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    collison_test(sensor_data, 200);
}


/**
 * Run the statments as needed
 */
int main(void)
{
    //Initialize the LCD. This also clears the screen
   lcd_init();
    //collison();
   // square();
char ch  = 0x3B;
ch = ch << 5;

  // point = 0xFFFFFC00;
   //point = &mys;

   //point++;
  lcd_printf("Char: %.8x", ch);



	return 0;
}

