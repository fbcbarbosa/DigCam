/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 */

/* $Id: twitest.c,v 1.1 2002/12/18 22:35:38 joerg_wunsch Exp $ */

/*
 * Simple demo program that talks to a 24Cxx I²C EEPROM using the
 * builtin TWI interface of an ATmega device.
 */

/*
 * ---------------------------------------------------------------------------
 * The code modified and reorganized by Jaakko Ala-Paavola 2003/08/20
 * jap@iki.fi
 * ---------------------------------------------------------------------------
 */

#define ACK             0
#define NACK            1
#define CAM_ADDR        0x00

void I2CInit();
void I2CStart(void);
void I2CStop(void);
void I2CRestart(void);
void I2CNack(void);
void I2CAck(void);
unsigned char I2CRead(void);
unsigned char I2CWriteAck(unsigned char byte);
unsigned char I2CWriteByte(unsigned char device_addr, unsigned char reg_addr, unsigned char data);
unsigned char I2CReadByte(unsigned char device_addr, unsigned char reg_addr, unsigned char *data);
unsigned char I2CReadArray(unsigned char device_addr, unsigned char reg_addr, unsigned char *data, unsigned char length);