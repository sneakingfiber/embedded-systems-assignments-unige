/* 
 * File:   main.c
 * Author: fyder9
 *
 * Created on April 22, 2026, 12:45 PM
 */

#include "xc.h"
#include <stdio.h>
#include <stdlib.h>

int setup_uart1_mikrobus2() {
  TRISDbits.TRISD11 = 1; // setting RD11 as input for RX
  TRISDbits.TRISD0 = 0;  // setting RD0 as output for TX
  RPINR18bits.U1RXR = 75;
  // remapping the pin for U1RX to RD11 number 71
  // RPINR18 is the register for UART1 RX
  // 0x1B is the code for RD11 in the PPS (Peripheral Pin Select) system
  RPOR0bits.RP64R = 0x01;
  // remapping the pin for U1TX to RD0 number 72
  // RPOR0 is the register for Remappable Pin Output Register
  // 0x01 is the code for RD0 in the PPS system

  // setting up UART at 9600 baud rate
  U1BRG = 47;            // (7400000 / 4) / (16 ? 9600) ? 1
  U1MODEbits.UARTEN = 1; // enabling UART
  U1STAbits.UTXEN = 1;   // enabling UART-TX

  return 1;
}

int main() {
  ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0X0000;
  TRISAbits.TRISA0 = 0; // LD1 output mode
  LATAbits.LATA0 = 0;  // LD1 off
  if(setup_uart1_mikrobus2()) {
    
    while(1) {
    if(U1STAbits.URXDA) { // if data is available in the receive buffer
      char received_char = U1RXREG; // read the received character
      if(U1STAbits.UTXBF) { // if the transmit buffer is full
        while (U1STAbits.UTXBF); // wait until the transmit buffer is not full
      }
      U1TXREG = received_char; // send the received character back
    }

  };
  return 0;
  }
}

