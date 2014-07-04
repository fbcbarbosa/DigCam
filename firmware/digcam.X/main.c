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

#include "lib/picdev/picDev.h"  // Connections of the picDev board
#include "lib/picdev/pinOut.h"  // Pinout for the picDev Board

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
char buffer[32];
char hex[3];
char header[14];
char infoheader[40];
unsigned int cam_buffer[CAM_WIDTH];
//unsigned int cam_buffer[CAM_HEIGHT/10][CAM_WIDTH/10];

const char bmp_header[54] = {
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

                //            case COM_BMP:
                //                writeln("Creating TEST BMP file...");
                //                log();
                //                createheader(header,352,244);
                //                createinfoheader(infoheader,352,244);
                //                usart_putnumchars(header, 14);
                //                usart_putnumchars(infoheader, 40);
                //                sendtable();
                //                senddata();
                //                Delayms(4000);

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
                for (i = 0; i < 255; i++) {
                    Dec2Hex(i, hex);
                    sprintf(buffer, "\tReg 0x%s", hex), write(buffer);

                    Dec2Hex(CamRead(i), hex);
                    sprintf(buffer, " = 0x%s", hex), writeln(buffer);
                }

                writeln("Device Control Register List (Register 0xFF = 0x00)");
                CamWrite(0xFF, 0x00);
                for (i = 0; i < 255; i++) {
                    Dec2Hex(i, hex);
                    sprintf(buffer, "\tReg 0x%s", hex), write(buffer);

                    Dec2Hex(CamRead(i), hex);
                    sprintf(buffer, " = 0x%s", hex), writeln(buffer);
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
                writeln("Getting camera status...");
                status = CamStatus();

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

                sprintf(buffer, "WIDTH: \t\t%lu", status.WIDTH), writeln(buffer);
                sprintf(buffer, "HEIGHT: \t%lu", status.HEIGHT), writeln(buffer);

                sprintf(buffer, "NBYTELINE: \t%lu", status.NBYTELINE), writeln(buffer);
                sprintf(buffer, "NHSYNC: \t%lu", status.NHSYNC), writeln(buffer);
                sprintf(buffer, "NBYTETOTAL: \t%lu", status.NBYTETOTAL), writeln(buffer);

                break;

            case COM_TAKE_PIC:
                writeln("Taking picture...");

                log(); // tell software to save the data in a txt file

                for (i = 0; i < CAM_HEIGHT; i++) {

                    unsigned int Y;
                    unsigned int D7, D6, D5, D4, D3, D2, D1, D0;
                    int j;
                    
                    CamGetPixelRow(i);

                    cam_buffer[CAM_WIDTH - 1] = 0xFFFF;
                    cam_buffer[CAM_WIDTH - 2] = 0x0000;

                    for (j = 0; j < CAM_WIDTH; j++) {

                        Y = cam_buffer[j];

                        D7 = (Y & (1 << 0)) >> 0; // RB0
                        D6 = (Y & (1 << 14)) >> 14; // RB14
                        D5 = (Y & (1 << 13)) >> 13; // RB13
                        D4 = (Y & (1 << 12)) >> 12; // RB12
                        D3 = (Y & (1 << 11)) >> 11; // RB11
                        D2 = (Y & (1 << 10)) >> 10; // RB10
                        D1 = (Y & (1 << 9)) >> 9; // RB8
                        D0 = (Y & (1 << 8)) >> 8; // RB9

                        char ch = D7 << 7 | D6 << 6 | D5 << 5 | D4 << 4 | D3 << 3 | D2 << 2 | D1 << 1 | D0 << 0;

                        writech(ch);
                        //sprintf(buffer, "%d ", Y), write(buffer);
                    }
                }

//                cam_buffer[CAM_HEIGHT - 1][CAM_WIDTH - 1] = 0xFFFF;
//                cam_buffer[CAM_HEIGHT - 1][CAM_WIDTH - 2] = 0x0000;
//
//                int j;
//                for (i = 0; i < CAM_HEIGHT; i++) {
//                    for (j = 0; j < CAM_WIDTH; j++) {
//                        unsigned int Y;
//                        unsigned int D7, D6, D5, D4, D3, D2, D1, D0;
//
//                        Y = cam_buffer[i][j];
//
//                        D7 = (Y & (1 << 0)) >> 0; // RB0
//                        D6 = (Y & (1 << 14)) >> 14; // RB14
//                        D5 = (Y & (1 << 13)) >> 13; // RB13
//                        D4 = (Y & (1 << 12)) >> 12; // RB12
//                        D3 = (Y & (1 << 11)) >> 11; // RB11
//                        D2 = (Y & (1 << 10)) >> 10; // RB10
//                        D1 = (Y & (1 << 9)) >> 9; // RB8
//                        D0 = (Y & (1 << 8)) >> 8; // RB9
//
//                        char ch = D7 << 7 | D6 << 6 | D5 << 5 | D4 << 4 | D3 << 3 | D2 << 2 | D1 << 1 | D0 << 0;
//
//                        writech(ch);
//                        //sprintf(buffer, "%d ", Y), write(buffer);
//                    }
//                }

                //                for (i = 0; i < CAM_HEIGHT; i++) {
                //
                //                    CamTakePic(i);
                //
                //                    for (j = 0; j < CAM_WIDTH; j++) {
                //
                //                        unsigned char VH;
                //                        unsigned char VL;
                //                        unsigned D7, D6, D5, D4, D3, D2, D1, D0;
                //
                //                        D7 = (cam_buffer_h[j] & (1 << 0)) >> 0; // RB0
                //                        D6 = (cam_buffer_h[j] & (1 << 14)) >> 14; // RB14
                //                        D5 = (cam_buffer_h[j] & (1 << 13)) >> 13; // RB13
                //                        D4 = (cam_buffer_h[j] & (1 << 12)) >> 12; // RB12
                //                        D3 = (cam_buffer_h[j] & (1 << 11)) >> 11; // RB11
                //                        D2 = (cam_buffer_h[j] & (1 << 10)) >> 10; // RB10
                //                        D1 = (cam_buffer_h[j] & (1 << 9)) >> 9; // RB8
                //                        D0 = (cam_buffer_h[j] & (1 << 8)) >> 8; // RB9
                //
                //                        VH = (unsigned char) (D7 << 7 | D6 << 6 | D5 << 5 | D4 << 4 | D3 << 3 | D2 << 2 | D1 << 1 | D0 << 0);
                //
                //                        D7 = (cam_buffer_l[j] & (1 << 0)) >> 0; // RB0
                //                        D6 = (cam_buffer_l[j] & (1 << 14)) >> 14; // RB14
                //                        D5 = (cam_buffer_l[j] & (1 << 13)) >> 13; // RB13
                //                        D4 = (cam_buffer_l[j] & (1 << 12)) >> 12; // RB12
                //                        D3 = (cam_buffer_l[j] & (1 << 11)) >> 11; // RB11
                //                        D2 = (cam_buffer_l[j] & (1 << 10)) >> 10; // RB10
                //                        D1 = (cam_buffer_l[j] & (1 << 9)) >> 9; // RB8
                //                        D0 = (cam_buffer_l[j] & (1 << 8)) >> 8; // RB9
                //
                //                        VL = (unsigned char) (D7 << 7 | D6 << 6 | D5 << 5 | D4 << 4 | D3 << 3 | D2 << 2 | D1 << 1 | D0);
                //
                //                        //VL = (VH << 7) | ((VL & 0xC0) >> 1) | (VL & 0x1f);
                //                        //VH = VH >> 1;
                //
                //                        writech(VL);
                //                        writech(VH);
                //                    }
                //                }

                writeln("\nDone!");

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