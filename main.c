#include "msp430g2553.h"

#define TXLED BIT0
#define RXLED BIT6
#define TXD BIT2
#define RXD BIT1

const char string[] = { "Recibido\r\n"};
unsigned int i; //Counter

int main(void)
{
   WDTCTL = WDTPW + WDTHOLD; // Stop WDT
   DCOCTL = 0; // Select lowest DCOx and MODx settings
   BCSCTL1 = CALBC1_1MHZ; // Set DCO
   DCOCTL = CALDCO_1MHZ;
   P2DIR = 0x0F; //P2 como salida en sus 4 bajos
   P2OUT = 0; //Saco un 0 por el puerto 2
   P2DIR |= 0xFF; // All P2.x outputs
   P2OUT &= 0x00; // All P2.x reset
   P1SEL |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
   P1SEL2 |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
   P1DIR |= RXLED + TXLED;
   P1OUT &= 0x00;
   UCA0CTL1 |= UCSSEL_2; // SMCLK
   UCA0BR0 = 104; // 1MHz 9600
   UCA0BR1 = 0; // 1MHz 9600
   UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1
   UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
   UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt
   //
   __bis_SR_register(GIE); // Enter LPM0 w/ int until Byte RXed
   while (1)
   { }
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
   P1OUT |= TXLED;
     UCA0TXBUF = string[i++]; // TX next character
    if (i == sizeof string - 1) // TX over?
       UC0IE &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
    P1OUT &= ~TXLED; }

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
   P1OUT |= RXLED;
    if (UCA0RXBUF == '1') // 'a' received?
    	{
		   i = 0;
		   UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
		   //UCA0TXBUF = string[i++];
		   P2OUT ^= BIT0;
    	}
    if (UCA0RXBUF == '2') // 'a' received?
        {
           i = 0;
           UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
           //UCA0TXBUF = string[i++];
           P2OUT ^= BIT1;
        }
    if (UCA0RXBUF == '3') // 'a' received?
        {
           i = 0;
           UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
           //UCA0TXBUF = string[i++];
           P2OUT ^= BIT2;
        }
    if (UCA0RXBUF == '4') // 'a' received?
        {
           i = 0;
           UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
           //UCA0TXBUF = string[i++];
           P2OUT ^= BIT3;
        }
    if (UCA0RXBUF == '5') // 'a' received?
        {
           i = 0;
           UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
           //UCA0TXBUF = string[i++];
           P2OUT ^= BIT4;
        }
    P1OUT &= ~RXLED;
}
