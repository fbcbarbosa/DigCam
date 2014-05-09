/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <p24Fxxxx.h>        // Device header file
#include <stdint.h>          // For uint32_t definition
#include <stdbool.h>         // For true/false definition

#include "user.h"            // Variables/params used by user.c
#include "UART1.h"           // UART1 peripheral functions

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
    
    // Turn off analogue functions on all pins
    AD1PCFG = 0xFFFF;

    /* Initialize peripherals */
    UART1Init();
}

/**
 * Read UART1 data received and transmit it back.
 */
void RepeaterProcessEvents() {
    unsigned char data = 0;

    // wait for data to be received
    data = UART1GetChar();

    // send data back on UART TX line
    UART1PutChar(data);
}

