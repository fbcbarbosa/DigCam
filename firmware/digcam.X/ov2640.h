/*
 * File:   ov2640.h
 * Authors: Fernando Barbosa, João Lucas e Roberto Walter
 *
 * Created on May 10, 2014, 02:02 AM
 *
 * Description: OV2640 function file.
 */

#ifndef OV2640_H
#define	OV2640_H

/******************************************************************************/
/* #define Macros                                                             */
/******************************************************************************/

// Camera Pin Map
#define CAM_SCL     _RD10
#define CAM_SDA     _RD9
#define CAM_PCLK    _RD0
#define CAM_HREFX   _RF3
#define CAM_XVCLK   _RB15    
#define CAM_VSYNC   _RD11
#define CAM_PWDN    _RB3     
#define CAM_RESET   _RB4

// Camera definitions
#define CAM_WIDTH      1600
#define CAM_HEIGHT     1200

// Slave address
#define CAM_ADDR        0x30

// Camera registers
#define CAM_REG_COM7    0x12    // Common Control 7

/******************************************************************************/
/* OV2640 Functions                                                           */
/******************************************************************************/

struct cam_status {
    int POWER;
    int RESET;
    int PCLK;
    int HREF;
    int VSYNC;

    long WIDTH;
    long HEIGHT;

    long NBYTELINE;
    long NHSYNC;

    long NBYTETOTAL;
};

struct cam_status CamStatus();

void CamInit();
void CamReset();
void CamTurnOn();
void CamTurnOff();
void CamReadPixelRow(int r0, unsigned int buffer[CAM_WIDTH]);
void CamWrite(unsigned char reg_addr, unsigned char data);
unsigned char CamRead(unsigned char reg_addr);

#endif