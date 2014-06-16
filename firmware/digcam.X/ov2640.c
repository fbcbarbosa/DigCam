/*
 * File:   ov2640.c
 * Author: Fernando Barbosa
 *
 * Created on May 10, 2014, 02:02 AM
 *
 * Description: camera isnstruction file.
 */

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "uart1.h"
#include "ov2640.h"
#include "i2c_cam.h"
#include "lib/picdev/picDev.h"  // Connections of the picDev board
#include "lib/picdev/pinOut.h"  // Pin mapping of the picDev Board

/******************************************************************************/
/* Commands #define Macros                                                    */
/******************************************************************************/

#define camSCL     _RD10
#define camSDA     _RD9
#define camPCLK    _RD0
#define camHREFX   _RF3
#define camXVCLK   _RB0
#define camVSYNC   _RD11
#define camPWDN    _RB2     // must be 1 to activate camera
#define camRESET   _RB4

#define camD7      _RB15
#define camD6      _RB14
#define camD5      _RB13
#define camD4      _RB12
#define camD3      _RB11
#define camD2      _RB10
#define camD1      _RB9
#define camD0      _RB8
#define camY1      _RB5
#define camY0      _RB8

#define camDATA    (unsigned char)(camD7 << 7 | camD6 << 6) // camD5 camD4 camD3 camD2 camD1 camD0]

/******************************************************************************/
/* OV2640 Functions                                                           */
/******************************************************************************/

// NOTES: SSBC SIO_C = I2C SCL
// NOTES: SSBC SIO_D = I2C SDA

void InitCamera() {
    CamWrite(0x12, CamRead(0x12)|0X80);
}

void TurnCameraOn()
{

}

void TurnCameraOff()
{
    
}

void TakePicture() {
    int y, r, h;
    
    for (y = 0; y < 352; y++) {
        while (camVSYNC);
        while (!camVSYNC);
        for (r = 0; r < 244; r++) {
            while (!camHREFX);
            for (h = 0; h < y; h++) {
                while (camPCLK);
                while (!camPCLK);
            }
            UART1PutChar(camDATA);
            while (camHREFX);
        }
    }
}