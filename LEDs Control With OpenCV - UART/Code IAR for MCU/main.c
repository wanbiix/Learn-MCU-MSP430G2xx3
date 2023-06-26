#include <msp430g2553.h>

#define LED1 BIT1
#define LED2 BIT2
#define LED3 BIT3
#define LED4 BIT4
#define LED5 BIT5

#define SMCLK_F 1000000 // frequency of Sub-System Master Clock in 1Mhz
#define BAUDRATE 9600 

void UART_Init();

void main() {
  WDTCTL = WDTPW + WDTHOLD; 
  
  P2DIR |= LED1 + LED2 + LED3 + LED4 + LED5;
  P2OUT |= (LED1 + LED2 + LED3 + LED4 + LED5);

  BCSCTL1 = CALBC1_1MHZ; 
  DCOCTL = CALDCO_1MHZ;
  UART_Init();
  
  _BIS_SR(GIE);
  
  while (1) { 
    unsigned char dieukhien;
    while (!(IFG2 & UCA0TXIFG)); 
    dieukhien = UCA0RXBUF;
    if (dieukhien == '0')
    {
      P2OUT &=~ (LED1 + LED2 + LED3 + LED4 + LED5);
    }
    if (dieukhien == '1')
    {
      P2OUT |= LED1; 
      P2OUT &=~ LED2;
      P2OUT &=~ LED3;
      P2OUT &=~ LED4;
      P2OUT &=~ LED5;
    }
    if (dieukhien == '2')
    {
      P2OUT |= LED1;
      P2OUT |= LED2;
      P2OUT &=~ LED3;
      P2OUT &=~ LED4;
      P2OUT &=~ LED5;
    }
    if (dieukhien == '3')
    {
      P2OUT |= LED1;
      P2OUT |= LED2;
      P2OUT |= LED3;
      P2OUT &=~ LED4;
      P2OUT &=~ LED5;
    }
    if (dieukhien == '4')
    {
      P2OUT |= LED1;
      P2OUT |= LED2;
      P2OUT |= LED3;
      P2OUT |= LED4;
      P2OUT &=~ LED5;
    }
    if (dieukhien == '5')
    {
      P2OUT |= LED1;
      P2OUT |= LED2;
      P2OUT |= LED3;
      P2OUT |= LED4;
      P2OUT |= LED5;
    }      
  }
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {  
  __bic_SR_register_on_exit (CPUOFF);
}

void UART_Init()
{
    P1SEL   |= BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
    P1SEL2  |= BIT1 + BIT2;
    UCA0CTL0 =0 ;		//Tat Parity ,LSB first,8-bit data,one stop bits
    UCA0CTL1|= UCSSEL_2; // SMCLK
    UCA0BR0 = 104;
    UCA0BR1 = 0;
    UCA0MCTL |= UCBRS_1;
    UCA0CTL1 &=~ UCSWRST; // **Initialize USCI state machine**
}
