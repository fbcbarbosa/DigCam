/*
 * File:   ov2640.h
 * Author: Fernando Barbosa
 *
 * Created on May 10, 2014, 02:02 AM
 *
 * Description: defines all commands known to the pic. Each command is define by
 * two hexadecimal characters, for a total of 256 commands.
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
#define CAM_D7      _RB0        
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
#define CAM_WIDTH      400 // 128
#define CAM_HEIGHT     148 // 96

// Slave address
#define CAM_ADDR        0x30

// Camera registers
#define CAM_REG_COM7    0x12    // Common Control 7

/******************************************************************************/
/* OV2640 Functions                                                           */
/******************************************************************************/

struct cam_reg {
    unsigned char reg;
    unsigned char val;
};

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
void CamWriteArray(const struct cam_reg camlist[]);
unsigned char CamRead(unsigned char reg_addr);

#endif