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

#include "lib/picdev/picDev.h"  // Connections of the picDev board
#include "lib/picdev/pinOut.h"  // Pin mapping of the picDev Board

#include "i2c.h"
#include "delay.h"
#include "main.h"
#include "ov2640.h"
#include "uart1.h"

#include <stdio.h>

/******************************************************************************/
/* Commands #define Macros                                                    */
/******************************************************************************/

// Camera Pin Map
#define CAM_SCL     _RD10
#define CAM_SDA     _RD9
#define CAM_PCLK    _RD0
#define CAM_HREFX   _RF3
#define CAM_XVCLK   _RB15       // _RB0
#define CAM_VSYNC   _RD11
#define CAM_PWDN    _RB3     // must be 1 to activate CAM_era
#define CAM_RESET   _RB4
#define CAM_D7      _RB0        //_RB15
#define CAM_D6      _RB14
#define CAM_D5      _RB13
#define CAM_D4      _RB12
#define CAM_D3      _RB11
#define CAM_D2      _RB10
#define CAM_D1      _RB9
#define CAM_D0      _RB8
#define CAM_Y1      _RB5
#define CAM_Y0      _RB2

// Camera definitions
#define CAM_DATA        (unsigned char)(CAM_D7 << 7 | CAM_D6 << 6 | CAM_D5 << 5 | CAM_D4 << 4 | CAM_D3 << 3 | CAM_D2 << 2 | CAM_D1 << 1 | CAM_D0 << 1)
#define CAM_WIDTH       1600
#define CAM_HEIGHT      1200
#define CAM_PCLK_FREQ   400     // KHZ

// Slave address
#define CAM_READ_ADDR   0x61
#define CAM_WRITE_ADDR  0x60

// Camera registers
#define CAM_REG_COM7    0x12    // Common Control 7

/******************************************************************************/
/* OV2640 Functions                                                           */
/******************************************************************************/

// NOTES: SSBC SIO_C = I2C SCL
// NOTES: SSBC SIO_D = I2C SDA

unsigned char *i2c_data;

unsigned char CamWrite(unsigned char reg_addr, unsigned char data);
unsigned char CamRead(unsigned char reg_addr, unsigned char *data);

/**
 * Initializes pin connections and turns on the Camera.
 */
void CamInit() {
    // Output
    _TRISB15 = 0; // XVCLK
    _TRISB3 = 0; // PWDN
    _TRISB4 = 0; // RESET

    // Input
    _TRISD0 = 1; // PCLK
    _TRISF3 = 1; // HREFX
    _TRISD11 = 1; // VSYNC
    _TRISB0 = 1; // D7
    _TRISB14 = 1; // D6
    _TRISB13 = 1; // D5
    _TRISB12 = 1; // D4
    _TRISB11 = 1; // D3
    _TRISB10 = 1; // D2
    _TRISB9 = 1; // D1
    _TRISB8 = 1; // D0
    _TRISB5 = 1; // Y1
    _TRISB2 = 1; // Y0

    // Clock output (XVCLK)
    REFOCON = 0x9000;

    // Init PCLK (uses Timer2)
    PR2 = (int) 16 * 1000 / (2 * CAM_PCLK_FREQ);
    IPC1bits.T2IP = 5; //set interrupt priority
    T2CON = 0b1000000000000000; //turn on the timer
    IFS0bits.T2IF = 0; //reset interrupt flag
    IEC0bits.T2IE = 1; //turn on the timer1 interrupt

    // Turn on camera
    CamTurnOn();
}

/**
 *
 */
void CamReset() {
    CamWrite(CAM_REG_COM7, 0x80);
    //CamWrite(CAM_REG_COM7, CamRead(CAM_REG_COM7)|0X80);
}

int CamIsOn() {
    if(CAM_PWDN) {
        return 0;
    }
    else {
        return 1;
    }
}

int CamIsReset() {
    if(CAM_RESET) {
        return 0;
    }
    else {
        return 1;
    }
}
/**
 *
 */
void CamTakePic() {
    CamRead(CAM_REG_COM7, i2c_data);

    //    writeln("Starting to take picture...");
    //    int y, r, h;
    //
    //    for (y = 0; y < CAM_WIDTH; y++) {
    //        sprintf(buf, "y = %d", y);
    //        writeln(buf);
    //        for (r = 0; r < CAM_HEIGHT; r++) {
    //            sprintf(buf, "\tr = %d", r);
    //            writeln(buf);
    //            while (!CAM_HREFX);
    //            for (h = 0; h < y; h++) {
    //                sprintf(buf, "\t\th = %d", h);
    //                writeln(buf);
    //                while (CAM_PCLK);
    //                while (!CAM_PCLK);
    //            }
    //            sprintf(buf, "\t\t\tD7 = %d", (int)CAM_D7);
    //            writeln(buf);
    //            //writech(CAM_D7);
    //            //writech(CAM_DATA);
    //            while (CAM_HREFX);
    //        }
    //    }
}

/**
 *
 */
void CamTurnOn() {
    debugmsg("CAM_PDWM -> 0");
    CAM_PWDN = 0;
    
    // Hardware reset sequency
    //    CAM_RESET = 0;
    //    Delayms(200);
    debugmsg("CAM_RESET -> 1");
    CAM_RESET = 1;
}

/**
 *
 */
void CamTurnOff() {
    debugmsg("CAM_PDWM -> 1");
    CAM_PWDN = 1;
}

/**
 * Write on camera register.
 * @param reg_addr
 * @param data
 * @return error value
 */
unsigned char CamWrite(unsigned char reg_addr, unsigned char data) {
    unsigned char error = 0;
    error = I2CWriteByte(CAM_WRITE_ADDR, reg_addr, data);

    if(error)
    {
        debugmsg("In CamWrite");
        sprintf(buffer, "reg_addr %d", reg_addr), debugmsg(buffer);
        sprintf(buffer, "data %d", data), debugmsg(buffer);
        sprintf(buffer, "error %d", error), debugmsg(buffer);
    }

    return error;
}

/**
 * Read camera register.
 * @param reg_addr  register address
 * @param *data     buffer where to store the data
 * @return error value
 */
unsigned char CamRead(unsigned char reg_addr, unsigned char *data) {
    unsigned char error = 0;
    error = I2CReadByte(CAM_READ_ADDR, reg_addr, data);

    if(error)
    {
        debugmsg("In CamRead");
        sprintf(buffer, "reg_addr %d", reg_addr), debugmsg(buffer);
        sprintf(buffer, "data %s", data), debugmsg(buffer);
        sprintf(buffer, "error %d", error), debugmsg(buffer);
    }

    return error;
}

void __attribute__((__interrupt__, auto_psv)) _T2Interrupt(void) {
    CAM_PCLK = ~CAM_PCLK;
    IFS0bits.T2IF = 0;
}