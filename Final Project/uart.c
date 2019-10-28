
#include "uart.h"


#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80


//Initializes UART1
void uart_init(void)    {


    //Set up GPIO
    //enable clock to GPIO, R1 = port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    //enable clock to UART1, R1 = UART1. ***Must be done before setting Rx and Tx (See DataSheet)
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;

    //Enable alternate function for PA0, PA1, and set functionality
    GPIO_PORTB_AFSEL_R |= (BIT1 | BIT0);
    GPIO_PORTB_PCTL_R |= 0x00000011;

    //Set digital or analog mode, and set pin directions
    GPIO_PORTB_DEN_R |= (BIT1 | BIT0);  //Set pins 0, 1 digital mode
    GPIO_PORTB_DIR_R &= ~BIT0;          // Set pin 0 (Rx) to input (0)
    GPIO_PORTB_DIR_R |= BIT1;           // Set pin 1 (Tx) to output (1)


    //Set up UART device
    //Configure UART functionality, frame format and Baud Speed

    //Disable UART0 device while we set it up
     UART1_CTL_R &= ~UART_CTL_UARTEN; // search tm4c123gh6pm.h


    //Set baud rate
    UART1_IBRD_R = 8;
    UART1_FBRD_R = 44;
    UART1_CC_R = UART_CC_CS_SYSCLK;                     //Use system clock as UART clock source

    //Set frame format: use frame format specified above
    UART1_LCRH_R = UART_LCRH_WLEN_8;          //
/*
    //turn off uart1
     UART1_CTL_R &= ~(UART_CTL_UARTEN);
     //clear interrupt flags
     UART1_ICR_R = (UART_ICR_TXIC | UART_ICR_RXIC);
     //enable send and receive interrupts
     UART1_IM_R |= UART_IM_TXIM | UART_IM_RXIM;
     //set priority of usart1 interrupt to 1
     NVIC_PRI1_R |= 0x00200000;
     //set bit 6
     NVIC_EN0_R |= 0x00000040;
     //tell cpu to use ISR handler for uart1
     //IntRegister(INT_UART1, uart_Handler);
     //enable interrupts
*/
    //Re-enable UART1
    UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);

    //IntMasterEnable();                  //Globally allows CPU to service interrupts

}

void uart_Handler(void){

    //received a byte
   if(UART1_MIS_R & UART_MIS_RXMIS){

     char i = uart_receive();
     if(i == '\r'){
           uart_sendChar('\n');
           uart_sendChar('\r');
       }
     else {
           uart_sendChar(i);
           lcd_putc(i);
       }
     UART1_ICR_R |= UART_ICR_RXIC; //clear interrupt
    }
    //sent a byte
   else if(UART1_MIS_R & UART_MIS_TXMIS){

        UART1_ICR_R |= UART_ICR_TXIC; //clear interrupt
    }
}


//Sends a character from the microcontroller
void uart_sendChar(char data)   {

    while(UART1_FR_R & 0x20)    {
    }

    UART1_DR_R = data;
}

//Receives given character from putty terminal
char uart_receive(void) {
    char data = 0;

    while(UART1_FR_R & UART_FR_RXFE)    {
    }

    data = (char) (UART1_DR_R & 0xFF);

    return data;
}

/**
 * @brief sends an entire string of characters over UART1 module
 * @param data pointer to the first index of the string to be sent
 */
void uart_sendStr(const char *data)
{
    //until we reach a null character
    while (*data != '\0')
    {
        //send the current character
        uart_sendChar(*data);
        // increment the pointer.
        data++;
    }

}
