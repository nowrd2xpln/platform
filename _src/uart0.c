#include "uart0.h"
#include "rprintf.h"	///< Reduced printf.  STDIO uses lot of FLASH space & Stack space.

void myISR(void) __attribute__ ((interrupt("IRQ")));
void myISR(void)
{
//  NOTE: placing rprintf() before reading U0RBR somehow clears U0RBR
//	int buff = 0;
//	buff = U0IIR;
//	buff = U0LSR;
	uart1_putchar(U0RBR);
	//rprintf("myISR: start\n");
//	buff = U0RBR;
//	rprintf("myISR: U0RBR = %d, (%c)\n", buff, buff);
//	uart1_putchar(buff);

    //while(!(U0LSR & 0x01));

//    int buff = 0;
//    int buff2 = 0;
//    int status_U0LSR = 0;

	 //rprintf("myISR: U0RBR = %d, (%c)\n", U0RBR, U0RBR);

    //buff = U0IIR;
/*    buff = U0RBR;
    status_U0LSR = U0LSR;
    rprintf("myISR: 0x%x\n", status_U0LSR);
    if( !(status_U0LSR & 0x1) )
    {
	  //uart1_putchar(U0RBR);
    	uart1_putchar(buff);
    }
    else
      rprintf("myISR: uart1_putchar() did not send\n");
*/
    //while(!(U0LSR & 0x01));
/*    if( buff & 0x04)
    {
    	buff2 = U0RBR;
    }
    rprintf("myISR: U0RBR = %d, (%c)\n", buff2, buff2);
    uart1_putchar(buff2);

*/
/*    if((buff = U0IIR) & 0x01)
    {
    	switch(buff & 0x0E)
    	{
    	case 0x04:
    		//buff2 = uart0_getchar();
    		buff2 = U0LSR;
    		uart1_putchar(U0RBR);
    		break;
    	default:
    		break;
    	}
    }*/
/*
    uart1_putchar(U0RBR);
    buff = U0IIR;
    buff = U0LSR;
*/

//rprintf("myISR: buff = %c\n", buff);
//uart1_putchar(buff);
    VICVectAddr = 0x0; // Acknowledge that ISR has finished execution
//    rprintf("myISR: end\n");
}

void init_Uart0 ( unsigned long baudrate)
{
	unsigned long divider;

	PINSEL0 &= 0xfffffff0;	// Clear configuration of P0.0 and P0.1
	PINSEL0 |= 0x00000005;	// Enable RxD0 and TxD0
	U0LCR = 0x83;			// 8 bits, no Parity, 1 Stop bit; DLAB = 1

	divider = ( PERI_FREQ / 16 ) / baudrate ;
	U0DLM = divider / 256;
	U0DLL = divider % 256;

	U0FCR = 0x6; // reset Rx and Tx FIFO
	U0FCR = 0x1; // enable FIFO
	U0LCR = 0x03; // now make DLAB=0

	U0IER = 0x1; // to keep things simple just enable "Receive Data Available(RDA)" interrupt

	//----------Setup UART0 Interrupt-------------
	//Any Slot with Lower Priority than TIMER0's slot will suffice
	//VICVectAddr1 = (unsigned)myUart0_ISR; //Pointer Interrupt Function (ISR)
	VICVectAddr1 = (unsigned)myISR;
	VICVectCntl1 = 0x20 | 6;
	VICIntSelect=0x0; /* selecting IRQ */


	VICIntEnable |= (1<<6); //Enable Uart0 interrupt , 6th bit=1
}

int uart0_putchar (int ch)
{
	while (!(U0LSR & 0x20));
	return (U0THR = ch);
}

void uart0_putString (const char *string)
{
	char ch;

	while ((ch = *string)) {
		uart0_putchar(ch);
		string++;
	}
}

int uart0_getchar (void)
{
	while(!(U0LSR & 0x01)) {

	}
	return (U0RBR);
}

void init_Uart1 (unsigned long baudrate)
{
	unsigned long divider;

	//PINSEL0 &= ~((1<<8)|(1<<9));  // Clear bits P0.8 and P0.9
//	PINSEL0 |= 0x00050000;
	PINSEL0 |= (1<<18)|(1<<16);     // Enable RxD1 and TxD1
	U1LCR = 0x83;  // DLAB = 1 and 8n1

	divider = ( PERI_FREQ / 16 ) / baudrate ;
	U1DLM = divider / 256;
	U1DLL = divider % 256;
	U1LCR = 0x03;			// DLAB = 0

}

int uart1_putchar (int ch)
{
	while (!(U1LSR & 0x20));
	return (U1THR = ch);
}

void uart1_putString (const char *string)
{
	char ch;

	while ((ch = *string)) {
		uart1_putchar(ch);
		string++;
	}
}

int uart1_getchar (void)
{
	while(!(U1LSR & 0x01)) {

	} 
	return (U1RBR);
}
