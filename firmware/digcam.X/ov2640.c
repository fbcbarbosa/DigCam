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
#include "ov2640_regs.h"

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
long CamGetHeight();
long CamGetBytePerLine();

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

    // Clock output (XVCLK) 12 MHZ
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

    // by Fernando

    /* 40x30 */
    //        CamWrite(0xff, 0x01); // Select bank
    //        CamWrite(0x17, 0x11); // HREFST[10:3]
    //        CamWrite(0x18, 0x16); // HREFEND[10:3]
    //        CamWrite(0x32, 0x36); // Bit[5:3]: HREFEND[2:0]; Bit[2:0]: HREFST[2:0]
    //        CamWrite(0x19, 0x01); // VSTRT[9:2]
    //        CamWrite(0x1a, 0x05); // VEND[9:2]
    //        CamWrite(0x03, 0x0B); // Bit[3:2]: VEND[1:0]; Bit[1:0]: VSTRT[1:0]

    /* CIF mode */
//    CamWrite(0xff, 0x01); // Select bank
//    CamWrite(0x12, 0x10); // CIF mode
//    CamWrite(0x17, 0x11); // HREFST[10:3] (Default)
//    CamWrite(0x03, 0x06); // CIF default
//    CamWrite(0x18, 0x16); // HREFEND[10:3] (CIF default)
//    CamWrite(0x19, 0x00); // VSTRT[9:2] (CIF default)
//    CamWrite(0x1a, 0x97); // VEND[9:2] (Default)
//    CamWrite(0x32, 0x09); // Bit[5:3]: HREFEND[2:0]; Bit[2:0]: HREFST[2:0] (CIF default)
//    CamWrite(0x03, 0x06); // Bit[3:2]: VEND[1:0]; Bit[1:0]: VSTRT[1:0] (CIF default)

    /* Window
     * HREFST = 0001 0001 001 = 137
     * HREFEND = 0001 0110 001 = 177
     * VSTRT = 0000 0000 10 = 2
     * VEND = 1001 0111 01 = 605
     * HREFEND - HREFST = 400
     * VEND - VSTRT = 603
     */

    /* CIF mode */
//    CamWrite(0xff, 0x01); // Select bank
//    CamWrite(0x12, 0x10); // CIF mode
//    CamWrite(0x17, 0x11); // HREFST[10:3] (Default)
//    CamWrite(0x03, 0x06); // CIF default
//    CamWrite(0x18, 0x43); // HREFEND[10:3] (CIF default)
//    CamWrite(0x19, 0x00); // VSTRT[9:2] (CIF default)
//    CamWrite(0x1a, 0x97); // VEND[9:2] (Default)
//    CamWrite(0x32, 0x09); // Bit[5:3]: HREFEND[2:0]; Bit[2:0]: HREFST[2:0] (CIF default)
//    CamWrite(0x03, 0x06); // Bit[3:2]: VEND[1:0]; Bit[1:0]: VSTRT[1:0] (CIF default)

    /* Window
     * HREFST = 0001 0001 001 = 137
     * HREFEND = 0100 0011 001 = 537
     * VSTRT = 0000 0000 10 = 2
     * VEND = 1001 0111 01 = 605
     * HREFEND - HREFST = 400
     * VEND - VSTRT = 603
     */

//    CamWrite(0xff, 0x00); // Select bank
//    CamWrite(0x2c, 0xff); // Configure PCLK/VSYNC/HREFX as output
//    CamWrite(0x2e, 0xdf); // Configure PCLK/VSYNC/HREFX as output

    CamWriteArray(OV2640_JPEG_INIT);
    CamWriteArray(OV2640_YUV422);
    CamWriteArray(OV2640_JPEG);

    CamWrite(0xff, 0x01);
    CamWrite(0x15, 0x00);

    CamWriteArray(OV2640_320x240_JPEG);

    CamWrite(0xff, 0x01);
    CamWrite(0x11, 35); // divide XCLK by 36 (0,333 MHz)
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

    tempStatus.WIDTH = CAM_WIDTH;
    tempStatus.HEIGHT = CAM_HEIGHT;

    if (tempStatus.PCLK && tempStatus.HREF && tempStatus.VSYNC) {
        tempStatus.NBYTELINE = CamGetBytePerLine();
        tempStatus.NHSYNC = CamGetHeight();
        tempStatus.NBYTETOTAL = (long)tempStatus.NBYTELINE * (long)tempStatus.NHSYNC;
    } else {
        tempStatus.NBYTELINE = 0;
        tempStatus.NHSYNC = 0;
        tempStatus.NBYTETOTAL = 0;
    }

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

    for (i = 0; i < 50000000; i++) {
        if (temp != CAM_VSYNC) {
            return 1;
        }
    }

    return 0;
}

int CamCheckHREFX() {
    long i;
    unsigned char temp = CAM_HREFX;

    for (i = 0; i < 50000000; i++) {
        if (temp != CAM_HREFX) {
            return 1;
        }
    }

    return 0;
}
///**
// *
// */
//void CamTakePic(int y0) {
//    int r, c;
//    while (CAM_VSYNC);
//    while (!CAM_VSYNC);
//    for (r = 0; r < CAM_WIDTH; r++) {
//
//        while (!CAM_HREFX);
//        for (c = 0; c < y0; c++) {
//            while (CAM_PCLK);
//            while (!CAM_PCLK);
//        }
//        cam_buffer_h[r] = CAM_DATA;
//
//        while (CAM_PCLK);
//        while (!CAM_PCLK);
//
//        cam_buffer_l[r] = CAM_DATA;
//        while (CAM_HREFX);
//    }
//}

/**
 * Return Lume (Y) bytes.
 */
void CamTakePic() {
    int i, j;

    // Vsync
    while (!CAM_VSYNC);
    while (CAM_VSYNC);

    for (i = 0; i < CAM_HEIGHT; i++) {
        // wait for HREF high
        while (!CAM_HREFX);
        for (j = 0; j < CAM_WIDTH; j++) {
            while (CAM_PCLK);
            while (!CAM_PCLK);

            // read first px (Y) when pclk low
            //cam_buffer[i][j] = PORTB;
            
            // discard Y/U bit
            while (CAM_PCLK);
            while (!CAM_PCLK);
        }
        while (CAM_HREFX);
    }
}

/**
 * Return Lume (Y) bytes.
 */
void CamGetPixelRow(int r0) {
    int r;

    // Vsync
    while (!CAM_VSYNC);
    while (CAM_VSYNC);

    // get to line r0
    for (r = 0; r < r0; r++) {
        // wait for Hsync
        while (!CAM_HREFX);
        while (CAM_HREFX);
    }

    // wait for HREF high
    while (!CAM_HREFX);

    for (r = 0; r < CAM_WIDTH; r++) {
        while (CAM_PCLK);
        while (!CAM_PCLK);

        // read first px (Y) when pclk low
        cam_buffer[r] = PORTB;

        // discard Y/U bit
        while (CAM_PCLK);
        while (!CAM_PCLK);
    }
}

//void CamTakePic(int r0) {
//    int r, c;
//
//    // Vsync
//    while (CAM_VSYNC);
//    while (!CAM_VSYNC);
//
//    // get to line r0
//    for (r = 0; r < r0; r++) {
//        // wait for Hsync
//        while (!CAM_HREFX);
//        while (CAM_HREFX);
//    }
//
//    // wait for valid data
//    while (!CAM_HREFX);
//
//    for (c = 0; c < CAM_WIDTH; c++) {
//        while (CAM_PCLK);
//        while (!CAM_PCLK);
//
//        // read first px (HIGH)
//        cam_buffer_h[c] = PORTB;
//
//        while (CAM_PCLK);
//        while (!CAM_PCLK);
//
//        // read second px (LOW)
//        cam_buffer_l[c] = PORTB;
//    }
//}

unsigned int CamReadPixel(int r0, int c0) {
    unsigned int temp;
    int r, c;
    r = 0;
    c = 0;

    // Vsync
    while (!CAM_VSYNC);
    while (CAM_VSYNC);

    // Count rows
    for (r = 0; r < r0; r++) {
        while (!CAM_HREFX);
        while (CAM_HREFX);
    }

    // Hsync - wait for valid data
    while (!CAM_HREFX);

    for (c = 0; c < c0; c++) {
        while (CAM_PCLK); // wait for PCLK = 0
        while (!CAM_PCLK);
    }

    // read when PCLK is low!
    while (CAM_PCLK);
    //while (!CAM_PCLK);

    temp = PORTB;
    return temp;
}

//RGB
//unsigned int CamReadPixel(int r0, int c0) {
//    unsigned int temp;
//    int r, c;
//    r = 0;
//    c = 0;
//
//    // Vsync
//    while (CAM_VSYNC);
//    while (!CAM_VSYNC);
//
//    // Count rows
//    for (r = 0; r < r0; r++) {
//        while (!CAM_HREFX);
//        while (CAM_HREFX);
//    }
//
//    // Hsync - wait for valid data
//    while (!CAM_HREFX);
//
//    for (c = 0; c < c0; c++) {
//        while (CAM_PCLK); // wait for PCLK = 0
//        while (!CAM_PCLK);
//    }
//
//    // read data at rising edge!
//    while (CAM_PCLK);
//    while (!CAM_PCLK);
//
//    temp = PORTB;
//    return temp;
//}

//    unsigned char temp = 0x00;
//    unsigned char temp_last = 0x00;
//    int i = 0x00;
//
//    while (CAM_VSYNC);
//    while (!CAM_VSYNC);
//    temp = CAM_DATA;
//    while( ((temp != 0xD9) || (temp_last != 0xFF)) && (i < 6000) )
//    {
//      while (CAM_PCLK);
//      while (!CAM_PCLK);
//      temp_last = temp;
//      temp = CAM_DATA;
//      cam_buffer[i] = temp;
//      i++;
//    }

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
void CamWriteArray(const struct cam_reg camlist[]) {
    unsigned char reg_addr = 0;
    unsigned char reg_val = 0;

    const struct cam_reg *next = camlist;

    while ((reg_addr != 0xFF) | (reg_val != 0xFF)) {
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

long CamGetBytePerLine() {
    long w = 0;

    // wait for initial pulse
    while (!CAM_VSYNC);
    while (CAM_VSYNC);

    while (CAM_HREFX);
    while (!CAM_HREFX);

    for (w = 0;CAM_HREFX; w++) {
        while (CAM_PCLK);
        while (!CAM_PCLK);

        // read first px (Y) on PCLK rising edge
        // do it to verify if clock is fast enough
    }

    return w;
}

long CamGetHeight() {
    long h = 0;

    // wait for initial pulse
    // WARNING: VSYNC oposite to datasheet (only if not using default config)
    while (!CAM_VSYNC);
    while (CAM_VSYNC);

    // while valid data
    while (!CAM_VSYNC) {
        // start counting horizontal syncs!
        while (!CAM_VSYNC && !CAM_HREFX);
        h++;
        while (CAM_HREFX);
    }
    h--;

    return h;
}