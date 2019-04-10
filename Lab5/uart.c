#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "uart.h"
#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80
#define INT_UART1 22
volatile char *s_data;
volatile int *flagg;
void serial_init(){


    // Turn on clock for GPIO PortB and UART1
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;


    // Enable alternate function for PB0,PB1 and set functionality
    GPIO_PORTB_AFSEL_R |= (0x3); // UART0 TX and RX
    GPIO_PORTB_PCTL_R |= 0x00000011; //Set port B pins 0,1 to Rx,Tx


    //Set digital or analog mode, and pin directions
    GPIO_PORTB_DEN_R |= (BIT1 | BIT0); //Set pins0,1 digital mode
    GPIO_PORTB_DIR_R &= ~BIT0; // Set pin 0 (Rx) to input(0)
    GPIO_PORTB_DIR_R |= BIT1;  // Set pin 1 (Tx) to output(1)


    //Set up UART device
    // A. Configure UART functionality, frame format and Baud speed
    //Disable UART0 device while we set it up
    UART1_CTL_R &= ~UART_CTL_UARTEN; // searchtm4c123gh6pm.h
    // Set desired UART functionality
    //UART1_CTL_R = 0b001000000000; // Receive enable, TX disabled
    //Set baud rate (9600 Baud)
    UART1_IBRD_R = 104; //16,000,000 / (16 * 9600) = 104.16666 104// 8
    UART1_FBRD_R = 11;  // .1666*64+.5 = 11.16666 11// 44
    UART1_CC_R =0;//UART_CC_CS_SYSCLK; //Use system clock as UART clock source 0xFFFFFFF0
    //Set frame format: useframe format specified above
    UART1_LCRH_R = UART_LCRH_WLEN_8; //0x60


//INterupts commented out for testing
//    //Clear interupts
//    UART1_ICR_R |= UART_ICR_RXIC;//clear interupt
//    UART1_IM_R  |= 0b00010000;
//
//    // A. Configure NVIC to allow UART interrupts
//   NVIC_PRI1_R |= 0x00200000;//Set UART1 IRQ pri to 1(bits 23-21)
//   NVIC_EN0_R |= 0x00000040;//Enable UART0 IRQ (IRQ 5)
//
//      //3// B. Bind UART0 interrupt requests to user’s Interrupt Handler
//     IntRegister(INT_UART1, My_UART1_Handler);
//       //Re-enable UART1
//    IntMasterEnable();//Globally allows CPU to service interrupts
    UART1_CTL_R = (UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE);

}
void intit_interupts(volatile char *s, volatile int *flag){

    s_data = s;// Set the locations of my volatiles
    flagg = flag;
}
void My_UART1_Handler(void){
   // char my_char= UART1_DR_R;

    *flagg =1;
    *s_data = uart_receive(); // Set the incoming data to volatile
    if(UART1_MIS_R & UART_MIS_RXMIS){//Recieved a byte
        //uart_receive();
        UART1_ICR_R |= UART_ICR_RXIC;//clear interupt
    }




}

void uart_sendChar(char data){

    while(UART1_FR_R & 0x20){
    }

    UART1_DR_R = data;
}


void uart_sendstr(char data[]){


    int i =0;
    while(data[i] != 0){//Send over chars one by one
        uart_sendChar(data[i]);
        i++;
    }
}

char uart_receive(void){
    char data =0;

   while(UART1_FR_R & UART_FR_RXFE){// Wait for text

    }
    data = (char) (UART1_DR_R & 0xFF);// data is equal text

    return data;
}
