/* 
 * File:   commands.h
 * Author: Fernando Barbosa
 * List of commands known to the PIC.
 *
 * Created on May 9, 2014, 11:23 PM
 *
 * Description: defines all commands known to the pic. Each command is define by
 * two hexadecimal characters, for a total of 256 commands.
 */

#ifndef COMMANDS_H
#define	COMMANDS_H

/******************************************************************************/
/* Commands #define Macros                                                    */
/******************************************************************************/

#define COM_NONE        0x00
#define COM_ON          0x01
#define COM_OFF         0x02
#define COM_PHOTO       0x03
#define COM_RESET       0x04
#define COM_STATUS      0x05
#define COM_READ0       0x06
#define COM_READ1       0x07
#endif	/* COMMANDS_H */