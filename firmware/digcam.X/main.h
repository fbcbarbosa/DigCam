/* 
 * File:   main.h
 * Author: fernando
 *
 * Created on June 20, 2014, 4:20 PM
 */

#ifndef MAIN_H
#define	MAIN_H

extern int debugging;
extern char buffer[64];

/******************************************************************************/
/* UART shortcuts                                                             */
/******************************************************************************/

#define debugmsg(s)         debugging ? UART1PutString(" >>>"),UART1PutString((char*)s),UART1PutString("\r\n") : UART1PutString("")
#define write(s)            UART1PutString((char*)s)
#define writeln(s)          UART1PutString((char*)s);UART1PutString("\r\n")
#define writech(ch)         UART1PutChar((char)ch)
#define read()              UART1GetString()
#define readch()            UART1GetChar()
#define ack()               UART1PutChar(254)
#define end()               UART1PutChar(255)

#endif	/* MAIN_H */

