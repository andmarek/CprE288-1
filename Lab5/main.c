/**
 * main.c
 * @author: marcanio, ziska
 * @date:
 */

#include "lcd.h"
#include "uart.h"
#include "button.h"

volatile int flag = 0; // flag updated by ISR
volatile char str[21];
volatile char s;

int main() {
    char data = 0;
    lcd_init();
    serial_init();
    intit_interupts(&s, &flag);

    int i =0;


    while (1)
    {

        if (flag)
        {
            flag =0;

            if (s != '\r')
            { //If it is not 20 then keep loading in
                str[i] = s;

                str[i+1] = '\0';
                i++;
                //uart_sendChgar(s);
            }
            if ((s == '\r') || (strlen(str) == 20))
            {
                uart_sendstr(str);
                uart_sendChar('\r');
                uart_sendChar('\n');
                lcd_printf("%s", str);
                i=0;
            }
        }
    }

    return 0;
}

//

//int main()
//{
//
//
//    char str[21];// Chars being filled
//    char data =0;// Data being inputted
//    int i =0;
//    lcd_init();
//    serial_init();
//    //button_init();
//
//
//    while (1==1)
//    {
//        data = uart_receive();
//        uart_sendChar(data);
//        if((data == '\r') || (strlen(str) == 20)){ //Check for 20 chars or enter
//            //int j =0;// Counter for printing on terminal
//               //lcd_printf("");
//
//              //while(j <=i){// Print each char one at a time
//
//              //uart_sendstr(str);
//              uart_sendChar('\r');
//              uart_sendChar('\n');
//
//               //}
//               lcd_printf("%s", str);
//               memset(str, 0, sizeof(str));
//               i=0;
//        }
//        else if(data != 0){//If it is not 20 then keep loading in
//               str[i] = data;
//               i++;
//              // uart_sendChar(data);
//        }
//        //lcd_printf("%c", data);
//    }
//
//return 0;
//
//}



