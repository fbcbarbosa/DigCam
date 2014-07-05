/*
 * File: i2c.h
 * Authors: Fernando Barbosa, João Lucas e Roberto Walter
 *
 * Description: opens I2C peripheral for serial communication as master.
 */

#define ACK             0
#define NACK            1

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