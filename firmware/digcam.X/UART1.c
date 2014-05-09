/*
 * File:   UART1.c
 * Author: Fernando Barbosa
 * Serial communication library. Based on picDev library by Arthur Benemann.
 *
 * Created on April 23, 2014, 2:33 PM
 *
 * Description: opens UART1 peripheral for serial communication. Contains some
 * basic input/output routines.
 */

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "UART1.h"          // Header file
#include "p24Fxxxx.h"       // Compiler definitions

#include "lib/picdev/picDev.h"  // Connections of the picDev board
#include "lib/picdev/pinOut.h"  // Pin mapping of the picDev Board

/******************************************************************************/
/* Pre-processing code                                                        */
/******************************************************************************/

#if (BAUD_RATE_ERROR_PCT > 3)
#error UART frequency error is worse than 3%
#elif (BAUD_RATE_ERROR_PCT > 2)
#warning UART frequency error is worse than 2%
#endif

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/**
 * UART1 initiation function.
 */
void UART1Init() {
    char trash;

    // set up registers
    U1BRG = BAUD_RATE_REG;
    U1MODE = U_MODE;
    U1STA = U_STA;

    //TRISFbits.TRISF4 = 1;   // configure the RX pin (RF4 - RP10)
    //TRISFbits.TRISF5 = 0;   // configure the TX pin (RF5 - RP17)

    _U1RXR = UART1_RX;      // map RX for picDev board
    UART1_TX = Ro_U1TX;     // map TX for picDev board

    // delete trash
    while (U1STAbits.URXDA)
        trash = U1RXREG;
    return;
}

/**
 * UART1 receive function.
 * @return the received character.
 */
char UART1GetChar() {
    U1STAbits.OERR=0;
    while (!U1STAbits.URXDA); // wait untill data arrives
    return U1RXREG;
}

/**
 * UART1 char transmit function.
 * @param ch Character to send.
 */
void UART1PutChar(char ch) {
    while (U1STAbits.UTXBF); // transmit ONLY if TX buffer is empty
    U1TXREG = ch;
}

/**
 * UART1 string transmit function.
 * @param string
 */
void UART1PutString(char *string) {
    while (*string) {
        UART1PutChar(*string++); // sends char and points to the next one
    };
}


