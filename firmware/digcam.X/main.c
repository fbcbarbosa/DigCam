/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <p24Fxxxx.h>       // Device header file

#include <stdio.h>          // String management functions
#include <stdint.h>         // Includes uint16_t definition
#include <stdbool.h>        // Includes true/false definition

#include "bmp.h"
#include "commands.h"       // List of commands known to the PIC
#include "delay.h"
#include "i2c.h"
#include "main.h"
#include "ov2640.h"
#include "system.h"         // System funct/params, like osc/peripheral config
#include "uart1.h"          // UART1 peripheral functions

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void); // I/O and Peripheral Initialization
void Dec2Hex(int n, char* hex); // Converts Decimal to Hexadecimal
#include "lib/picdev/picDev.h"  // Connections of the picDev board
#include "lib/picdev/pinOut.h"  // Pinout for the picDev Board
#include "commands.h"           // Pin mapping of the picDev Board

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void); // I/O and Peripheral Initialization
void Dec2Hex(int n, char* hex); // Converts Decimal to Hexadecimal

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

int debugging = 0;
int p = 100;
char buffer[64];
char hex[3];

char header[14];
char infoheader[40];

const char BMP_HEADER[54] =
{
      0x42, 0x4D, 0x36, 0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
      0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void) {
    // initialize IO ports and peripherals
    InitApp();

    // main loop
    while (1) {
        struct cam_status status;

        unsigned char chCommand;
        unsigned char chParameter;

        // wait for command
        chCommand = readch();
        chParameter = readch();

        // acknolowdges command
        ack();

        switch (chCommand) {
            int i;

            case COM_BMP:
                writeln("Creating TEST BMP file...");
                log();
                createheader(header,352,244);
                createinfoheader(infoheader,352,244);
                usart_putnumchars(header, 14);
                usart_putnumchars(infoheader, 40);
                sendtable();
                senddata();
                Delayms(4000);

            case COM_GET_P:
                sprintf(buffer, "The value of parameter p is %d", p), writeln(buffer);
                break;

            case COM_NONE:
                break;

            case COM_READ:
                Dec2Hex(chParameter, hex);
                sprintf(buffer, "Reading register 0x%s", hex), writeln(buffer);
                Dec2Hex(CamRead(chParameter), hex);
                sprintf(buffer, "Value: 0x%s", hex), writeln(buffer);
                break;

            case COM_READALL:
                writeln("Printing registers: ");
                
                writeln("Device Control Register List (Register 0xFF = 0x01)");
                CamWrite(0xFF, 0x01);
                for(i = 0; i < 255; i++)
                {
                    Dec2Hex(i, hex);
                    sprintf(buffer, "\tReg 0x%s", hex), write(buffer);
                    
                    Dec2Hex(CamRead(i), hex);
                    sprintf(buffer, " = 0x%s", hex), writeln(buffer);
                }
                
                writeln("Device Control Register List (Register 0xFF = 0x00)");
                CamWrite(0xFF, 0x00);
                for(i = 0; i < 255; i++)
                {
                    Dec2Hex(i, hex);
                    sprintf(buffer, "\tReg 0x%s, ", hex), write(buffer);

                    Dec2Hex(CamRead(i), hex);
                    sprintf(buffer, " = 0x%s}", hex), writeln(buffer);
                }

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
                status =  CamStatus();

                write("POWER: \t");
                if (status.POWER) {
                    writeln("ON");
                } else {
                    writeln("OFF");
                }

                write("RESET: \t");
                if (status.RESET) {
                    writeln("ON");
                } else {
                    writeln("OFF");
                }

                write("PCLK: \t");
                if (status.PCLK) {
                    writeln("ON");
                } else {
                    writeln("OFF");
                }

                 write("HREF: \t");
                if (status.PCLK) {
                    writeln("ON");
                } else {
                    writeln("OFF");
                }

                write("VSYNC: \t");
                if (status.PCLK) {
                    writeln("ON");
                } else {
                    writeln("OFF");
                }

                break;

            case COM_TAKE_PIC:
                writeln("Taking picture...");
                log();  // tell software to save the data in a txt file
                createheader(header,352,244);
                createinfoheader(infoheader,352,244);
                usart_putnumchars(header, 14);
                usart_putnumchars(infoheader, 40);
                sendtable();
                CamTakePic();
                Delayms(4000); // delay will announce data stream is over
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

/**
 * Function to convert decimal to hexadecimal.
 * @param n Decimal number between 0 and 255.
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