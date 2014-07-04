/* 
 * File:   main.h
 * Author: fernando
 *
 * Created on June 20, 2014, 4:20 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include "ov2640.h"

extern int debugging;
extern char buffer[32];
extern unsigned int cam_buffer[CAM_WIDTH];
//extern unsigned int cam_buffer[CAM_HEIGHT/10][CAM_WIDTH/10];

/******************************************************************************/
/* UART shortcuts                                                             */
/******************************************************************************/

#define debugmsg(s)         debugging ? UART1PutString(" >>>"),UART1PutString((char*)s),UART1PutString("\r\n") : UART1PutString("")
#define write(s)            UART1PutString((char*)s)
#define writeln(s)          UART1PutString((char*)s);UART1PutString("\r\n")
#define writech(ch)         UART1PutChar((char)ch)
#define read()              UART1GetString()
#define readch()            UART1GetChar()
#define log()               UART1PutChar(253)
#define ack()               UART1PutChar(254)
#define end()               UART1PutChar(255)

/******************************************************************************/
/* Bit/Byte operations                                                        */
/******************************************************************************/

#define pgm_read_word(x)        ( ((*((unsigned char *)x + 1)) << 8) + (*((unsigned char *)x)))

#endif	/* MAIN_H */

