/*
 * File: i2c.c
 * Authors: Fernando Barbosa, Jo�o Lucas e Roberto Walter
 *
 * Description: opens I2C peripheral for serial communication as master.
 */

#define I2CWRITE    0
#define I2CREAD     1

#ifndef I2CSPEED
#define I2CSPEED    37          // 400KHz
#endif

#include "lib/picdev/pinOut.h"  // Pinout for the PicDev Board

#include "i2c.h"
#include "delay.h"
#include "main.h"
#include "uart1.h"

#include <i2c.h>
#include <stdio.h>

/**
 * Initiates I2C1 module to baud rate BRG.
 */
void I2CInit() {   
    // 7bit addressing
    I2C1CONbits.A10M = 0;
    I2C1CONbits.SCLREL = 0;

    // Clean slave addresses
    I2C1ADD = 0;
    I2C1MSK = 0;

    // Disable SMBus
    I2C1CONbits.SMEN = 0;

    // Baud rate setting
    I2C1BRG = I2CSPEED;

    // Enable I2C module
    I2C1CONbits.I2CEN = 1;
}

/**
 * Initiates Start condition on SDAx and SCLx pins. Hardware clear at end of
 * master Start sequence.
 */
void I2CStart(void) {
    I2C1CONbits.SEN = 1;
    while (I2C1CONbits.SEN);
}

/**
 * Initiates Stop condition on SDAx and SCLx pins. Hardware clear at end of
 * master Stop sequence.
 */
void I2CStop(void) {
    I2C1CONbits.PEN = 1;
    while (I2C1CONbits.PEN);
}

/**
 * Initiates Repeated Start condition on SDAx and SCLx pins. Hardware clear at 
 * end of master Repeated Start sequence.
 */
void I2CRestart(void) {
    I2C1CONbits.RSEN = 1;
    while (I2C1CONbits.RSEN);
}

/**
 * Initiates NOT Acknowledge sequence.
 */
void I2CNack(void) {
    I2C1CONbits.ACKDT = 1; // set for NotACk
    I2C1CONbits.ACKEN = 1; // send Nack
    while (I2C1CONbits.ACKEN); // wait for ACK to complete
    I2C1CONbits.ACKDT = 0;
}

/**
 * Initiates Acknowledge sequence on SDAx and SCLx pins and transmits ACKDT data
 * bit. Hardware clear at end of master Acknowledge seque
 */
void I2CAck(void) {
    I2C1CONbits.ACKDT = 0; // set for ACk
    I2C1CONbits.ACKEN = 1; // send Ack
    while (I2C1CONbits.ACKEN); // wait for ACK to complete
}

/**
 * Basic I2C byte send.
 * @param byte
 * @return
 */
unsigned char I2CWriteAck(unsigned char byte) {
    I2C1TRN = byte;
    while (I2C1STATbits.TRSTAT == 1);
    return (!I2C1STATbits.ACKSTAT);
}

/**
 * Reads data, returns the read data, no ack.
 */
unsigned char I2CRead(void) {
    I2C1CONbits.RCEN = 1; // enable Master receive
    while (!I2C1STATbits.RBF); // wait for receive bufer to be full
    return (I2C1RCV); // return data in buffer
}

/**
 * I2C write function.
 * @param device_addr
 * @param sub_addr
 * @param data
 * @return
 */
unsigned char I2CWriteByte(unsigned char device_addr, unsigned char reg_addr, unsigned char data) {
    unsigned char error = 0;
    I2CStart();
    error += !I2CWriteAck((device_addr << 1) | I2CWRITE);
    error += !I2CWriteAck(reg_addr);
    error += !I2CWriteAck(data);
    I2CStop();
    return error;
}

/**
 * I2C read function.
 * @param device_addr
 * @param reg_addr
 * @param data
 * @return
 */
unsigned char I2CReadByte(unsigned char device_addr, unsigned char reg_addr, unsigned char *data) {
    unsigned char error = 0;
    I2CStart();
    error += !I2CWriteAck((device_addr << 1) | I2CWRITE);
    error += !I2CWriteAck(reg_addr);
    I2CStart();
    error += !I2CWriteAck((device_addr << 1) | I2CREAD);

    if (error) {
        I2CStop();
        return error;
    }

    *data = I2CRead();
    I2CStop();
    return error;
}