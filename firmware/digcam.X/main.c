/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <p24Fxxxx.h>       // Device header file

#include <stdio.h>          // String management functions
#include <stdint.h>         // Includes uint16_t definition
#include <stdbool.h>        // Includes true/false definition

#include "commands.h"       // List of commands known to the PIC
#include "delay.h"
#include "i2c.h"
#include "ov2640.h"
#include "system.h"         // System funct/params, like osc/peripheral config
#include "uart1.h"          // UART1 peripheral functions

#include "lib/picdev/picDev.h"  // Connections of the picDev board
#include "lib/picdev/pinOut.h"  // Pinout for the picDev Board
#include "commands.h"           // Pin mapping of the picDev Board

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* i.e. uint16_t <variable_name>; */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void); // I/O and Peripheral Initialization

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
int debugging = 1;
int p = 100;
char buffer[64];

int16_t main(void) {
    // initialize IO ports and peripherals
    InitApp();

    // main loop
    while (1) {
        char chCommand;
        char chParameter;

        // wait for command
        chCommand = readch();
        chParameter = readch();

        // acknolowdges command
        ack();

        switch (chCommand) {
            case COM_GET_P:
                sprintf(buffer, "The value of parameter p is %d", p);
                writeln(buffer);
                break;

            case COM_NONE:
                break;

            case COM_RESET:
                writeln("Reseting camera...");
                CamReset();
                break;

            case COM_SET_P:
                writeln("P is set!");
                p = (uint8_t) chParameter;
                break;

            case COM_STATUS:
                if (CamIsOn()) {
                    writeln("The camera is on!");
                }
                else {
                    writeln("The camera is off!");
                }

                if (CamIsReset()) {
                    writeln("The camera is on reset!");
                }
                else {
                    writeln("The camera is not on reset!");
                }
                break;
                
            case COM_TAKE_PIC:
                writeln("Taking picture...");
                CamTakePic();
                writeln("Done!");
                break;

            case COM_TURN_OFF:
                writeln("Turning off camera...");
                CamTurnOff();
                writeln("Done!");
                break;

            case COM_TURN_ON:
                writeln("Turning on camera...");
                CamTurnOn();
                writeln("Done!");
                break;

            default:
                break;
        }

        // informs that command has concluded
        end();
    };
}

/**
 * Setup analog functionality and port direction and initialize peripherals.
 */
void InitApp(void) {
    /* Setup analog functionality and port direction */
    AllPinsDigital();

    /* Initialize peripherals */
    UART1Init();
    I2CInit();
    CamInit();
}