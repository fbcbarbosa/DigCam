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
/* User Functions                                                             */
/******************************************************************************/
/**
 * UART1 initiation function.
 */
void UART1Init() {
//    if (BAUD_RATE_ERROR_PCT > 3)
//        printf("ERROR: UART frequency error is worse than 3%");
//    else if (BAUD_RATE_ERROR_PCT > 2)
//        printf("WARNING UART frequency error is worse than 2%");

    char trash;

    // set up registers
    U1BRG = BAUD_RATE_REG;
    U1MODE = U_MODE;
    U1STA = U_STA;

    // reset RX interrupt flag
    IFS0bits.U1RXIF = 0;

    Ri_U1RX = UART1_RX;
    UART1_TX = Ro_U1TX;

// interrupt request enabled
//    _U1TXIE = 1;
//    _U1RXIE = 1;

    while (U1STAbits.URXDA)
        trash = U1RXREG;

    return;
}

/**
 * UART1 transmit function.
 * @param Ch Character to send.
 */
void UART1PutChar(char Ch) {
    // transmit ONLY if TX buffer is empty
    while (U1STAbits.UTXBF);
    U1TXREG = Ch;
}

/**
 * UART1 receive function.
 * @return the received character.
 */
char UART1GetChar() {
    char Temp;

    _U1TXIE = 1;

    // wait for buffer to fill up, wait for interrupt
    while (!IFS0bits.U1RXIF);
    
    _U1TXIE = 0;

    Temp = U1RXREG;
    // reset interrupt
    IFS0bits.U1RXIF = 0;
    // return my received byte
    return Temp;
}