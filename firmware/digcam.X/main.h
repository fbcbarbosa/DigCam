/* 
 * File:   main.h
 * Authors: Fernando Barbosa, Jo�o Lucas e Roberto Walter
 *
 * Created on June 20, 2014, 4:20 PM
 */

#ifndef MAIN_H
#define	MAIN_H

/******************************************************************************/
/* UART shortcuts                                                             */
/******************************************************************************/

#define write(s)            UART1PutString((char*)s)
#define writeln(s)          UART1PutString((char*)s);UART1PutString("\r\n")
#define writech(ch)         UART1PutChar((char)ch)
#define read()              UART1GetString()
#define readch()            UART1GetChar()
#define log()               UART1PutChar(253)
#define ack()               UART1PutChar(254)
#define end()               UART1PutChar(255)

/******************************************************************************/
/* Global variables                                                           */
/******************************************************************************/

#define NROW_BUFF 10

extern int flagRX;
extern char strBuffer[16];

#endif	/* MAIN_H */

