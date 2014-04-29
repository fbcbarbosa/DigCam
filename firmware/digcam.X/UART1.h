/*
 * File:   UART1.h
 * Author: Fernando Barbosa
 * Serial communication library. Based on picDev library by Arthur Benemann.
 *
 * Created on April 23, 2014, 2:33 PM
 *
 * Description: opens UART1 peripheral for serial communication. Contains some
 * basic input/output routines.
 */

/******************************************************************************/
/* UART1 #define Macros                                                  */
/******************************************************************************/

#ifndef UART_H
#define	UART_H

#define	U_MODE	0x8008	// Enable UART, BREGH=1 (High-speed mode)
#define	U_STA	0x0400	// Set interrupts (maybe 0x8400)

#define BAUD_RATE           115200
#define BAUD_RATE_REG       (GetPeripheralClock()/4/BAUD_RATE - 1)
#define BAUD_RATE_ACTUAL    (GetPeripheralClock()/4/(BAUD_RATE_REG + 1))
#define BAUD_RATE_ERROR     ((BAUD_RATE_ACTUAL > BAUD_RATE) ? BAUD_RATE_ACTUAL - BAUD_RATE : BAUD_RATE - BAUD_RATE_ACTUAL)
#define BAUD_RATE_ERROR_PCT ((BAUD_RATE_ERROR*100+BAUD_RATE/2)/BAUD_RATE)

//#if (BAUD_RATE_ERROR_PCT > 3)
//#error UART frequency error is worse than 3%
//#elif (BAUD_RATE_ERROR_PCT > 2)
//#warning UART frequency error is worse than 2%
//#endif

#endif

/******************************************************************************/
/* UART1 Function Prototypes                                                   */
/******************************************************************************/

//Initiation
extern void UART1Init();

//UART transmit function
extern void  UART1PutChar(char Ch);

//UART receive function
extern char UART1GetChar();