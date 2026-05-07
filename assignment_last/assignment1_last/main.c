#include <p33EP512MU810.h>
#include <xc.h>
#include "timer.h"
#include "uart.h"
#include <string.h>
int ret;
void algorithm() {
  tmr_wait_ms(TIMER2, 7);
}
int main() {
 // make  LD1 RA0 
  TRISAbits.TRISA0 = 0; // Set RA0 as output (LD1)
// make LD2 RG9 
  TRISGbits.TRISG9 = 0; // Set RG9 as output (LD2)
  UART1_Init();
  tmr_setup_period(TIMER1, 10);
  tmr_setup_period(TIMER4, 500);
  //enable global interrupts
 __builtin_enable_interrupts();
 // make led 0
 LATAbits.LATA0 = 0; // Turn off LD1
 LATGbits.LATG9 = 0; // Turn off LD2
  
  while(1) {
    algorithm();
    // code to handle the assignment
    
    


    ret = tmr_wait_period(TIMER1);
  }
}