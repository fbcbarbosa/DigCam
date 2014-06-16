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

#ifndef _EE24XX__
#define _EE24XX_

#ifndef UCSRB
# ifdef UCSR1A          /* ATmega128 */
#  define UCSRA UCSR1A
#  define UCSRB UCSR1B
#  define UBRR UBRR1L
#  define UDR UDR1
# else /* ATmega8 */
#  define UCSRA USR
#  define UCSRB UCR
# endif
#endif
#ifndef UBRR
#  define UBRR UBRRL
#endif

#define TWI_SLA_CAM     0xc0    //Cam C0    sensor 90
#define MAX_ITER        200
#define PAGE_SIZE       8       // before 8

#define CAM_ADDR        0x00

void CamWrite(char reg, char value);
char CamRead(char reg);

void I2Cwrite(char addr, char subaddr, char value);
void I2CRead(char addr, char subaddr, char value);

void i2c_init(int clk);
void i2c_restart();
void i2c_start();
void i2c_reset_bus(void);
char i2c_read();
char i2c_read_ack();
char i2c_send_byte(int data);

char I2Cpoll(char addr);
#endif

