/*
 * File:   UART1.h
 * Authors: Fernando Barbosa, João Lucas e Roberto Walter
 *
 * Created on April 23, 2014, 2:33 PM
 *
 * Description: opens UART1 peripheral for serial communication using ISRs.
 * Contains some basic input/output routines.
 */

/******************************************************************************/
/* UART1 #define Macros                                                       */
/******************************************************************************/

#ifndef UART_H
#define	UART_H

#define	U_MODE	0x8008	// Enable UART, BREGH=1 (High-speed mode)
#define	U_STA	0x2400	// Set interrupts

#define BAUD_RATE           115200
#define BAUD_RATE_REG       (GetPeripheralClock()/4/BAUD_RATE - 1)
#define BAUD_RATE_ACTUAL    (GetPeripheralClock()/4/(BAUD_RATE_REG + 1))
#define BAUD_RATE_ERROR     ((BAUD_RATE_ACTUAL > BAUD_RATE) ? BAUD_RATE_ACTUAL - BAUD_RATE : BAUD_RATE - BAUD_RATE_ACTUAL)
#define BAUD_RATE_ERROR_PCT ((BAUD_RATE_ERROR*100+BAUD_RATE/2)/BAUD_RATE)
#endif

/******************************************************************************/
/* UART1 Function Prototypes                                                  */
/******************************************************************************/

//Initiation
void UART1Init();

//UART char receive function
char UART1GetChar();

//UART char transmit function
void UART1PutChar(char ch);

//UART string transmit function
int UART1PutString(char *string);

void UART1DisableInterrupt();

void UART1EnableInterrupt();

//ISR
void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void);
