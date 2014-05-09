//------------------------------------------------
//		picDev.h
//	picDev hardware configuration
//	Arthur Benemann 28/03/2012
//------------------------------------------------
//	Description:
//  Contais the connections of the picDev board on Define structures.
//-----------------------------------------------
#ifndef _PICDEV_HARDWARE
#define _PICDEV_HARDWARE

#include "p24Fxxxx.h"       // Compiler Definitions
#include "pinOut.h"         // Pin mapping of the picDev Board

#define _NOPSV  __attribute__((no_auto_psv))

//------------- Clock frequency values -----------------
//used for UART and SPI baud rate generation.
#define GetSystemClock()	(32000000ul)	// Hz
#define GetInstructionClock()	(GetSystemClock()/2)
#define GetPeripheralClock()	(GetSystemClock()/2)

//---------------------- IO ----------------------------
#define LED 		P49
#define LEDtris	 	P49tris

//------------------- UART -----------------------------
#define UART1_RX        Ri31        // U1RX at pin RP10
#define UART1_TX        Ro32        // U1TX at pin RP17


#endif
