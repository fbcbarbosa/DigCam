/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <p24Fxxxx.h>       // Device header file

#include <stdio.h>
#include <stdint.h>         // Includes uint16_t definition
#include <stdbool.h>        // Includes true/false definition

#include "commands.h"       // List of commands known to the PIC
#include "system.h"         // System funct/params, like osc/peripheral config
#include "uart1.h"          // UART1 peripheral functions
#include "ov2640.h"

#include "lib/picdev/picDev.h"  // Connections of the picDev board
#include "lib/picdev/pinOut.h"
#include "commands.h"  // Pin mapping of the picDev Board

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

#define write(s)            UART1PutString((char*)s)
#define writeln(s)          UART1PutString((char*)s);UART1PutString("\r\n")
#define writech(ch)         UART1PutChar((char)ch)
#define read()              UART1GetString()
#define readch()            UART1GetChar()

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* i.e. uint16_t <variable_name>; */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void);                 // I/O and Peripheral Initialization
void RepeaterProcessEvents(void);   // Repeater main function

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
uint8_t p = 100;

int16_t main(void)
{
    // Initialize IO ports and peripherals
    InitApp();
  
    // Main loop
    while (1) {
        //RepeaterProcessEvents();

        char buf[33];
        char ch1, ch2;
        // ch1 = command
        // ch2 = parameter (optional)

        ch1 = readch();
        ch2 = readch();
        
        switch(ch1)
        {
            case COM_NONE:
                break;
            case COM_TURN_ON:
                writeln("ON");
                TurnCameraOn();
                break;
            case COM_TURN_OFF:
                writeln("OFF");
                TurnCameraOff();
                break;
            case COM_TAKE_PIC:
                writeln("PIC");
                TakePicture();
                break;
            case COM_GET_P:
                sprintf(buf, "The value of parameter p is %d", p);
                writeln(buf);
                break;
            case COM_SET_P:
                writeln("P is set!");
                p = (uint8_t)ch2;
                break;
            default:
                break;
        }
    };
}

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