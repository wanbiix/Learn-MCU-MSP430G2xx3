
#include "msp430g2553.h"

#define LED_PIN BIT0
#define BUTTON_PIN BIT4

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR |= LED_PIN;
  P2REN |= BUTTON_PIN;
  P2OUT |= BUTTON_PIN;
  
  while(1){
    if ((P2IN & BUTTON_PIN)==0){
      P1OUT ^= LED_PIN;
      __delay_cycles(300000);
    }
  }
}
