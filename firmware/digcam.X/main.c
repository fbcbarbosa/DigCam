/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <p24Fxxxx.h>       // Device header file

#include <stdint.h>         // Includes uint16_t definition
#include <stdbool.h>        // Includes true/false definition

#include "system.h"         // System funct/params, like osc/peripheral config
#include "user.h"           // User funct/params, such as InitApp
#include "UART1.h"          // UART1 peripheral functions

#include "lib/picdev/UART2.h"   // UART2 peripheral functions
#include "lib/picdev/picDev.h"  // Connections of the picDev board
#include "lib/picdev/pinOut.h"  // Pin mapping of the picDev Board

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* i.e. uint16_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{
    initU2();   // Initialize the Uart module

    lineU2();                       // Sends's a new line before anything
    putsU2("UART example.\r\n");    // Send's some welcome string.
    putsU2("Date: "__DATE__"\r\n"); // Send's the Date of compilation
    putsU2("Time: "__TIME__"\r\n"); // Send's the Time of compilation
    putsU2("Running the loop. Just type anything:\r\n");

    // Main Loop
    while (1) {
        putU2(getU2());
    };

    /* Configure the oscillator for the device */
    ///ConfigureOscillator();

    /* Initialize IO ports and peripherals */
//    InitApp();
//
//    UART1PutChar('T');
//    UART1PutChar('E');
//    UART1PutChar('S');
//    UART1PutChar('T');
//
//    while(1)
//    {
//        RepeaterProcessEvents();
//    }
}
