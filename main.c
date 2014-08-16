#include "lpc2xxx.h"	///< LPC Header Definitions
#include "uart0.h"		///< Uart0 Header file to initialize UART0
#include "rprintf.h"	///< Reduced printf.  STDIO uses lot of FLASH space & Stack space.
#include "lowLevel.h"

#include <stdio.h>
#include <malloc.h>

#define TIMER0 4
#define TIMER1 5
#define UART0 6
#define UART1 7

#define PLOCK 0x00000400
#define MR0I (1<<0) //Interrupt When TC matches MR0
#define MR0R (1<<1)	//Reset TC when TC matches MR0
#define MR0_DELAY_MS 500 //0.5 Second(s) Delay

/// Very rough delay of 1 ms.  Assuming 48Mhz CPU Clock.
void delay_ms(unsigned long count);
void myISR(void);

/*
void myUart0_ISR(void)
{
	long int regVal;
	regVal = U0IIR; // Reading U0IIR also clears it!
	
	//Recieve Data Available Interrupt has occured
	regVal = U0RBR; // dummy read
		
	IOPIN0 ^= (1<<3); // Toggle 4th Pin in PORT0 .. P0.3

	VICVectAddr = 0x0; // Acknowledge that ISR has finished execution
	
}	
*/

int main(void)
{
	lowLevelInit();
	wakeupInit();
	exceptionHandlerInit();

	int i = 0;
	
	init_Uart0(9600);
	init_Uart1(9600);
	int buff = 0;

	rprintf_devopen(uart0_putchar);
	//rprintf_devopen(uart1_putchar);

	delay_ms(2000);
	rprintf("-------------------------------------------------------------------------\n");
	rprintf("--                    System Boot complete...                          --\n");
	rprintf("-------------------------------------------------------------------------\n");

	printf("You can use printf but it will bloat your code!!!\n");
	//printf("Only STDIO printf can print floating-point numbers: %f\n", 1.23f);

	delay_ms(1000);
	printf("Test Memory allocation of 1024 bytes: ");
	char *memPointer = malloc(1024);
	printf("Memory pointer = %p\n", memPointer);
         	
    uart1_putchar('*');

	while (1)
	{
		rprintf("-- Hello World %i --\n", ++i);
		uart1_putchar('*');
		delay_ms(1000);
        //buff = uart1_getchar();
        //uart1_putchar(buff);
		
	}

	return 0;
}


void delay_ms(unsigned long count)
{
	volatile unsigned long c = count * 4000;

	// Each loop iteration should be:
	// 2 LOADS, 1 STORE, 1 CMP, 1 SUB, and 1 BRANCH
	// Around 16 instructions?
	while (--c)
		;
}
