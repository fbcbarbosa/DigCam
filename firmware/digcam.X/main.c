/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <p24Fxxxx.h>       // Device header file

#include <stdint.h>         // Includes uint16_t definition
#include <stdbool.h>        // Includes true/false definition

#include "system.h"         // System funct/params, like osc/peripheral config
#include "user.h"           // User funct/params, such as InitApp
#include "UART1.h"          // UART1 peripheral functions

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
    // Initialize IO ports and peripherals
    InitApp();

    writeln("");
    writeln("Date: "__DATE__);
    writeln("Time: "__TIME__);
    writeln("");
    writeln("Welcome to DigCam Interface!");
    writeln("");
    write("Select an option: ");

    // Main loop
    while (1) {
        writech(read());
        //RepeaterProcessEvents();
    };
}
