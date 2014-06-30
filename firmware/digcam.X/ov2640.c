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
#include "ov2640_regs.h"
#include "uart1.h"

#include <stdio.h>

/******************************************************************************/
/* OV2640 Functions                                                           */
/******************************************************************************/

// NOTES: SSBC SIO_C = I2C SCL
// NOTES: SSBC SIO_D = I2C SDA

unsigned char i2c_data;

int CamIsOn();
int CamIsReset();
int CamCheckPCLK();
int CamCheckHREFX();
int CamCheckVSYNC();

/**
 * Initializes pin connections and turns on the Camera.
 */
void CamInit() {
    // Output
    _TRISB3 = 0; // PWDN
    _TRISB4 = 0; // RESET
    
    // Input
    _TRISD0 = 1; // PCLK
    _TRISF3 = 1; // HREFX
    _TRISD11 = 1; // VSYNC

    // Input
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

    Nop();
    Nop();
    
    // Clock output (XVCLK)
    REFOCON = 0x9000;

    // Turn on camera
    CamTurnOn();
    CamReset();
}
/**
 * Software reset on the camera.
 */
void CamReset() {
    CamWrite(0xff, 0x01); // Select bank
    CamWrite(0x12, 0x80); // Reset

    Delayms(100);

    // JPEG
    //    CamWriteArray(OV2640_JPEG_INIT);
    //    CamWriteArray(OV2640_YUV422);
    //    CamWriteArray(OV2640_JPEG);
    //
    //    CamWrite(0xFF, 0x01);
    //    CamWrite(0x15, 0x00);
    //
    //    CamWriteArray(OV2640_320x240_JPEG);

    // BMP
    CamWriteArray(OV2640_QVGA);
}

int CamIsOn() {
    if (CAM_PWDN) {
        return 0;
    } else {
        return 1;
    }
}

int CamIsReset() {
    if (CAM_RESET) {
        return 0;
    } else {
        return 1;
    }
}

/**
 * Return camera status.
 * @return
 */
struct cam_status CamStatus() {
    struct cam_status tempStatus;

    tempStatus.POWER = CamIsOn();
    tempStatus.RESET = CamIsReset();
    tempStatus.PCLK = CamCheckPCLK();
    tempStatus.HREF = CamCheckHREFX();
    tempStatus.VSYNC = CamCheckVSYNC();

    return tempStatus;
}

int CamCheckPCLK() {
    long i;
    unsigned char temp = CAM_PCLK;

    for (i = 0; i < 50000; i++) {
        if (temp != CAM_PCLK) {
            return 1;
        }
    }

    return 0;
}

int CamCheckVSYNC() {
    long i;
    unsigned char temp = CAM_VSYNC;

    for (i = 0; i < 50000; i++) {
        if (temp != CAM_VSYNC) {
            return 1;
        }
    }

    return 0;
}

int CamCheckHREFX() {
    long i;
    unsigned char temp = CAM_HREFX;

    for (i = 0; i < 50000; i++) {
        if (temp != CAM_HREFX) {
            return 1;
        }
    }

    return 0;
}

/**
 *
 */
void CamTakePic() {
    //int high_bit;
    //int low_bit;
    //RGB565 cada px é 16 bits = 2bytes
    int y, r, h;
    for (y = 0; y < CAM_WIDTH; y++) {
        while (CAM_VSYNC);
        while (!CAM_VSYNC);
        for (r = 0; r < CAM_HEIGHT; r++) {
            while (!CAM_HREFX);
            for (h = 0; h < y; h++) {
                while (CAM_PCLK);
                while (!CAM_PCLK);
            }
            writech(CAM_DATA);
            while (CAM_HREFX);
        }
    }

//    for (y = 0; y < CAM_WIDTH/100; y++) {
//        CAM_MVSYNC = 0;
//        Delayus(200);
//        CAM_MVSYNC = 1;
//        for (r = 0; r < CAM_HEIGHT/100; r++) {
//            CAM_MHSYNC = 1;
//            Delayus(100);
//            CAM_MHSYNC = 0;
//            sprintf(buffer, "CAM_DATA %d", CAM_DATA), debugmsg(buffer);
//        }
//    }
}

/**
 *
 */
void CamTurnOn() {
    debugmsg("CAM_PDWM -> 0");
    CAM_PWDN = 0;

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
 */
void CamWrite(unsigned char reg_addr, unsigned char data) {
    unsigned char error = 0;
    error = I2CWriteByte(CAM_ADDR, reg_addr, data);

    debugmsg("In CamWrite");
    sprintf(buffer, "reg_addr %d", reg_addr), debugmsg(buffer);
    sprintf(buffer, "data %d", data), debugmsg(buffer);
    sprintf(buffer, "error %d", error), debugmsg(buffer);

    return;
}

/**
 * 
 * @param camlist
 */
void CamWriteArray(const struct cam_reg camlist[])
{
	unsigned char reg_addr = 0;
	unsigned char reg_val = 0;
        
	const struct cam_reg *next = camlist;

	while ((reg_addr != 0xFF) | (reg_val != 0xFF))
	{
		reg_addr = next->reg;
		reg_val = next->val;
		CamWrite(reg_addr, reg_val);
	   	next++;
	}
}

/**
 * Read camera register.
 * @param reg_addr  register address
 * @return data
 */
unsigned char CamRead(unsigned char reg_addr) {
    unsigned char error = 0;
    unsigned char data;

    error = I2CReadByte(CAM_ADDR, reg_addr, &data);

    debugmsg("In CamRead");
    sprintf(buffer, "reg_addr %d", reg_addr), debugmsg(buffer);
    sprintf(buffer, "data %d", data), debugmsg(buffer);
    sprintf(buffer, "error %d", error), debugmsg(buffer);

    return data;
}

/**
 * Read camera register.
 * @param reg_addr  register address
 * @return data
 */

void __attribute__((__interrupt__, auto_psv)) _T2Interrupt(void) {
    CAM_PCLK = ~CAM_PCLK;
    IFS0bits.T2IF = 0;
}