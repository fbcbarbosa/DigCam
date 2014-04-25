/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <p24Fxxxx.h>        /* Device header file */
#include <stdint.h>          /* For uint32_t definition */
#include <stdbool.h>         /* For true/false definition */
#include "user.h"            /* variables/params used by user.c */

#include "UART1.h"              // UART1 peripheral functions

#include "lib/picdev/picDev.h"  // Connections of the picDev board
#include "lib/picdev/pinOut.h"  // Pin mapping of the picDev Board

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/**
 * Setup analog functionality and port direction and initialize peripherals.
 */
void InitApp(void) {
    /* Setup analog functionality and port direction */
    
    //Set up I/O Port
    AD1PCFGL = 0xFFFF;      // set to all digital I/O
    TRISB = 0xF3FF;         // configure all PortB as input
    RPINR18bits.U1RXR = 2;  // UART1 receive set to RB2
    RPOR1bits.RP3R = 3;     // UART1 transmit set to RB3

    /* Initialize peripherals */
    UART1Init();
}

/**
 * Read UART1 data received and transmit it back.
 */
void RepeaterProcessEvents() {
    unsigned char data = 0;

    // wait for data to be received
    //data = UART1GetChar();

    // send data back on UART TX line
    UART1PutChar(data);
}

