#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

void serial_init();
void uart_sendChar(char data);
char uart_receive(void);
void uart_sendstr(char data[]);
void My_UART1_Handler(void);
void intit_interupts(volatile char *s, volatile int *flag);
void uart_senddata(const char *data);

