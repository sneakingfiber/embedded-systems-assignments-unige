
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"
/*
 *
 */

void algorithm(){
    tmr_wait_ms(TIMER2, 7);
}

void setupUART1() {
    // setting up UART at 9600 baud rate
    U1BRG = 11;            // (7400000 / 4) / (16 ? 9600) ? 1
    U1MODEbits.UARTEN = 1; // enable UART
    U1STAbits.UTXEN = 1; // enable UART transmission
}
void readUART1(){
    char buffer[10]; // buffer to store received data
    int counter = 0;

    while(U1STAbits.URXDA) { // while data is available in the receive buffer
        unsigned char received_char = U1RXREG; // read the received character
        buffer[counter] = received_char; // store the received character in the buffer
        counter++;
        if(counter == 6) { // if buffer is full
                buffer[counter] = received_char; // store the received character in the buffer
        }

    }
}

void validateMessage(char message[10]) {
    
    if(message[0] == '$'){
        if(message[1] == 'B' && message[2] == 'W' && message[3] == '.' &&
            message[4] != '\0' && message[5] != '\0' && message[6] != '\0' && message[7] == '*' ) {
        // message is valid and it's a BW message
        //check the numbers
        string number =  message[5] + message[6];
        int value = number.stoi();
        if(value >= 8 && value <= 15) {
            // value is valid
        } else {
            //send error message back to the sender
            U1TXREG = '$ERR,1'; // sending error message back to the sender
        }
        }
    }
    if(message[0] == '$'){
        if(message[1] == 'B' && message[2] == 'R' && message[3] == '.' &&
            message[4] != '\0' && message[5] != '\0' && message[6] == '*' ) {
        // message is valid and it's a BR message
        //check the numbers
        string number =  message[5] + message[6];
        int value = number.stoi();
        if(value >= 8 && value <= 15) {
            // value is valid
            // SET THE ACCELLEROMETERS BADWIDTH TO THE VALUE
        } else {
            //send error message back to the sender
            U1TXREG = '$ERR,1'; // sending error message back to the sender
        }
        }
        if(message[1]=='H' && message[2]== 'Z' && message[3] == '.' &&
            message[4] != '\0' && message[5] != '\0' && message[6] == '*' ) {
            // message is valid and it's a HZ message
            string number =  message[4] + message[5];
            int value = number.stoi();
            if(value == 0){
                //DISABLE THE MESSAGE
            }
            if(value == 1 || value == 2 || value == 5 || value = 10) {
                // SEND ACCELLEROMETERS DATA THROUGH UART
            } else {
                //send error message back to the sender
                U1TXREG = '$ERR,1'; // sending error message back to the sender
            }
        }
    }
}

int main(int argc, char** argv) {
    // disabling all analog ports
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0X0000;
    // enabling the tris for output
    TRISAbits.TRISA0 = 0;
    TRISGbits.TRISG9 = 0;
    TRISDbits.TRISD11 = 1; // setting RD11 as input for RX
    TRISDbits.TRISD0 = 0;  // setting RD0 as output for TX
    int counter = 0;
    tmr_setup_period(TIMER1, 10);

    while(1) {
        algorithm();

        counter ++;
        if(counter == 50) {
            LATAbits.LATA0 = !LATAbits.LATA0;
            counter = 0;
        }
    }
    

    
    
    
}

