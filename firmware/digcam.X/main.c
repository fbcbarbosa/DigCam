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
#include "main.h"
#include "ov2640.h"
#include "uart1.h"          // UART1 peripheral functions

#include "lib/picdev/picDev.h"  // Connections of the picDev board
#include "lib/picdev/pinOut.h"  // Pinout for the picDev Board

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void);             // I/O and Peripheral Initialization
void Dec2Hex(int n, char* hex); // Converts Decimal to Hexadecimal

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

int flagRX = 0;
char strBuffer[16];                 // buffer for UART messages
char hexBuffer[3];                  // buffer for hexadecimal strings
unsigned int camBuffer[CAM_WIDTH];  // buffer to store camera data

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void) {
    // initialize IO ports and peripherals
    InitApp();
    
    // main loop
    while (1) {
        // check for received char UART flag
        if (flagRX) {

            int i;
            struct cam_status status;
            unsigned char chCommand;

            // clear flag
            flagRX = 0;

            // wait for command
            chCommand = readch();

            // acknolowdges command
            ack();

            switch (chCommand) {                 
                case COM_NONE:
                    break;

                case COM_READ1:
                    writeln("Printing registers: ");
                    writeln("Device Control Register List (Register 0xFF = 0x01)");
                    CamWrite(0xFF, 0x01);
                    for (i = 0; i < 255; i++) {
                        Dec2Hex(i, hexBuffer);
                        sprintf(strBuffer, "\tReg 0x%s", hexBuffer), write(strBuffer);

                        Dec2Hex(CamRead(i), hexBuffer);
                        sprintf(strBuffer, " = 0x%s", hexBuffer), writeln(strBuffer);
                    }
                    break;

                case COM_READ0:
                    writeln("Device Control Register List (Register 0xFF = 0x00)");
                    CamWrite(0xFF, 0x00);
                    for (i = 0; i < 255; i++) {
                        Dec2Hex(i, hexBuffer);
                        sprintf(strBuffer, "\tReg 0x%s", hexBuffer), write(strBuffer);

                        Dec2Hex(CamRead(i), hexBuffer);
                        sprintf(strBuffer, " = 0x%s", hexBuffer), writeln(strBuffer);
                    }
                    break;

                case COM_RESET:
                    writeln("Reseting camera...");
                    CamReset();
                    break;

                case COM_STATUS:
                    writeln("Getting camera status...");

                    UART1DisableInterrupt();
                    status = CamStatus();
                    UART1EnableInterrupt();

                    write("POWER: \t\t");
                    if (status.POWER) {
                        writeln("ON");
                    } else {
                        writeln("OFF");
                    }

                    write("RESET: \t\t");
                    if (status.RESET) {
                        writeln("ON");
                    } else {
                        writeln("OFF");
                    }

                    write("PCLK: \t\t");
                    if (status.PCLK) {
                        writeln("ON");
                    } else {
                        writeln("OFF");
                    }

                    write("HREF: \t\t");
                    if (status.HREF) {
                        writeln("ON");
                    } else {
                        writeln("OFF");
                    }

                    write("VSYNC: \t\t");
                    if (status.VSYNC) {
                        writeln("ON");
                    } else {
                        writeln("OFF");
                    }
                    sprintf(strBuffer, "WIDTH: \t\t%lu", status.WIDTH), writeln(strBuffer);
                    sprintf(strBuffer, "HEIGHT: \t%lu", status.HEIGHT), writeln(strBuffer);
                    sprintf(strBuffer, "NBYTELINE: \t%lu", status.NBYTELINE), writeln(strBuffer);
                    sprintf(strBuffer, "NHSYNC: \t%lu", status.NHSYNC), writeln(strBuffer);
                    sprintf(strBuffer, "NBYTETOTAL: \t%lu", status.NBYTETOTAL), writeln(strBuffer);
                    break;

                case COM_PHOTO:
                    writeln("Taking picture...");

                    // tell software to save the data
                    log();

                    for (i = 0; i < CAM_HEIGHT; i++) {

                        unsigned int Y;
                        unsigned int D7, D6, D5, D4, D3, D2, D1, D0;
                        int j;

                        UART1DisableInterrupt();
                        CamReadPixelRow(i, camBuffer);
                        UART1EnableInterrupt();

                        for (j = 0; j < CAM_WIDTH; j++) {

                            Y = camBuffer[j];

                            D7 = (Y & (1 << 0)) << 7; // RB0  -> 7
                            D6 = (Y & (1 << 14)) >> 8; // RB14 -> 6
                            D5 = (Y & (1 << 13)) >> 8; // RB13 -> 5
                            D4 = (Y & (1 << 12)) >> 8; // RB12 -> 4
                            D3 = (Y & (1 << 11)) >> 8; // RB11 -> 3
                            D2 = (Y & (1 << 10)) >> 8; // RB10 -> 2
                            D1 = (Y & (1 << 9)) >> 8; // RB8  -> 1
                            D0 = (Y & (1 << 8)) >> 8; // RB9  -> 0

                            char ch = D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0;

                            writech(ch);
                        }
                    }

                    writeln("\nDone!");
                    break;

                case COM_OFF:
                    writeln("Turning off camera...");
                    CamTurnOff();
                    writeln("Done!");
                    break;

                case COM_ON:
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

/**
 * Function to convert a decimal number into a hexadecimal string.
 * @param n Decimal number between 0 and 255.
 * @param hex Hexadecimal char buffer.
 */
void Dec2Hex(int n, char* hex) {
    char temp[2] = "00";

    int i = 0, rem;
    while (n != 0) {
        rem = n % 16;
        switch (rem) {
            case 10:
                temp[i] = 'A';
                break;
            case 11:
                temp[i] = 'B';
                break;
            case 12:
                temp[i] = 'C';
                break;
            case 13:
                temp[i] = 'D';
                break;
            case 14:
                temp[i] = 'E';
                break;
            case 15:
                temp[i] = 'F';
                break;
            default:
                temp[i] = rem + '0';
                break;
        }
        ++i;
        n /= 16;
    }

    hex[0] = temp[1];
    hex[1] = temp[0];
    hex[2] = '\0';
}