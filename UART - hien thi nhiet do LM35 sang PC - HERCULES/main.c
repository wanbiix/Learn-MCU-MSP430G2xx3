#include <msp430g2553.h>
#include <intrinsics.h>
#include <string.h>
#include "stdio.h"
#include "stdlib.h"

#define SMCLK_F 1000000 // frequency of Sub-System Master Clock in 1Mhz
#define BAUDRATE 9600 // may be ... 1200, 2400, 4800, 9600, 19200,
//------------- Khai báo prototype hàm
void delayms(int ms);
void UART_Init();
void UART_Write_Char(unsigned char data);
void UART_Write_String(char* string);
void UART_Write_Int(unsigned long n);
//--------------------------------------
unsigned int nhietdo;

void main (void){
  WDTCTL = WDTPW + WDTHOLD;
  P2DIR |= 0x04; // P2.2 OUTPUT
  P2DIR |= 0x02; // P2.1 OUTPUT 
  
  BCSCTL1 = CALBC1_1MHZ; //chon tan so dao dong noi là 1Mhz 
  DCOCTL = CALDCO_1MHZ;
  
  UART_Init();   
  
  ADC10CTL0 = SREF_1 + ADC10SHT_2 + REFON + ADC10ON + ADC10IE;
  __delay_cycles (10000);  
  ADC10CTL1 = INCH_7; //chon kenh ngo vao la kenh A7 
  ADC10AE0 =  0x80; //P1.7 option select   
  CCTL0 = CCIE; //Cho phep ngat  
  TACCR0 = 32768;// tan so th?ch anh ngo?i  
  TACTL = TASSEL_1 + MC_1 + ID_1; 
  _BIS_SR(GIE);
  while (1){  
    ADC10CTL0 |= ENC + ADC10SC; //cho phép va bat dau chuyen doi    
    __delay_cycles (30);
    
    nhietdo = ((unsigned long) (ADC10MEM*1.5*100)/1024);
    
    if (nhietdo>35){    
      P2OUT |= BIT1; // P2.1 len 1    
    }    
    else{    
      P2OUT &=~ BIT1; // P2.1 xu?ng 0    
    }
    
    UART_Write_String("Nhiet do la:"); //In chuoi ki tu 
    UART_Write_Int(nhietdo);    
    UART_Write_Char(10); //In ki tu 
    
    unsigned char dieukhien;    
    while (!(IFG2 & UCA0TXIFG));    
    dieukhien = UCA0RXBUF; // TX --> RX    
    UCA0TXBUF = dieukhien; // MSP gui ki tu len PC    
    if (dieukhien== 'b'){ // led OFF    
      P2OUT &=~ BIT2;    
    }    
    if (dieukhien =='a'){ // led ON    
      P2OUT |= BIT2;    
    }    
    delayms (1000);    
  }//end while
}//end main

// Dinh nghia Vector ngat ADC10

#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR (void){
  __bic_SR_register_on_exit (CPUOFF);
}
//--------------------------------
//Init UART
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

/* hoac
void UART_Init()
{
    unsigned int tempfactor;
    P1SEL   |= BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
    P1SEL2  |= BIT1 + BIT2;
    UCA0CTL0 =0 ;		//Tat Parity ,LSB first,8-bit data,one stop bits
    UCA0CTL1|= UCSSEL_2; // SMCLK
    
    tempfactor = SMCLK_F/BAUDRATE;
    UCA0BR0 = (unsigned char) tempfactor&0x00FF;
    tempfactor >>= 8;
    UCA0BR1 = (unsigned char) (tempfactor&0x00FF);
    UCA0MCTL |= UCBRF_0 + UCBRS_0;

    UCA0CTL1 &=~ UCSWRST; // **Initialize USCI state machine**
}
*/

//---------------------
//In KyTu

void UART_Write_Char(unsigned char data)
{
  while (!(IFG2&UCA0TXIFG)); // Doi gui xong ky tu truoc
  UCA0TXBUF= data; // Moi cho phep gui ky tu tiep theo
}
//-------------------------
// In chuoi ky tu
void UART_Write_String(char* string)
{
  while(*string) // Het chuoi ky tu thi thoat
  {
    while (!(IFG2&UCA0TXIFG)); // Doi gui xong ky tu truoc
    UCA0TXBUF= *string; // Moi cho phep gui ky tu tiep theo
    string ++; // Lay ky tu tiep theo
  }
  UART_Write_Char(0);
}
//-------------------
void UART_Write_Int(unsigned long n){
   unsigned char buffer[16];
   unsigned char i,j;  
   if(n == 0) {
     UART_Write_Char('0');
      return;
   }
   for (i = 15; i > 0 && n > 0; i--) {
      buffer[i] = (n%10)+'0';
      n /= 10;
   }  
   for(j = i+1; j <= 15; j++) {
       UART_Write_Char(buffer[j]);
   }
}
void delayms (int ms){
  for (int i=0; i< ms; i++){
    __delay_cycles(1000);
  }
}