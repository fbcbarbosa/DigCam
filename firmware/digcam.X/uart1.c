/*
 * File:   UART1.c
 * Authors: Fernando Barbosa, João Lucas e Roberto Walter
 *
 * Created on April 23, 2014, 2:33 PM
 *
 * Description: opens UART1 peripheral for serial communication using ISRs.
 * Contains some basic input/output routines.
 */

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include "uart1.h"          // Header file
#include "main.h"          // Header file
#include "p24Fxxxx.h"       // Compiler definitions
#include "string.h"

#include "lib/picdev/picDev.h"  // Connections of the picDev board
#include "lib/picdev/pinOut.h"  // Pin mapping of the picDev Board

/******************************************************************************/
/* Pre-processing code                                                        */
/******************************************************************************/

#if (BAUD_RATE_ERROR_PCT > 3)
#error UART frequency error is worse than 3%
#elif (BAUD_RATE_ERROR_PCT > 2)
#warning UART frequency error is worse than 2%
#endif

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/
#define 	BUFFER_SERIAL_SIZE		128

char buffer_serial[BUFFER_SERIAL_SIZE];
int buffer_serial_counter = 0;
int is_busy = 0;

void UART1DisableInterrupt() {
    IEC0bits.U1RXIE = 0;
    IEC0bits.U1TXIE = 0;
}

void UART1EnableInterrupt() {
    IEC0bits.U1RXIE = 1;
    IEC0bits.U1TXIE = 1;
}

/**
 * UART1 initiation function.
 */
void UART1Init() {
    char trash;

    // set up registers
    U1BRG = BAUD_RATE_REG;
    U1MODE = U_MODE;
    U1STA = U_STA;
    
    IEC0bits.U1RXIE = 1;
    IEC0bits.U1TXIE = 1;

    TRISFbits.TRISF4 = 1; // configure the RX pin (RF4 - RP10)
    TRISFbits.TRISF5 = 0; // configure the TX pin (RF5 - RP17)

    _U1RXR = UART1_RX;  // map RX for picDev board
    UART1_TX = Ro_U1TX; // map TX for picDev board

    // delete trash
    while (U1STAbits.URXDA)
        trash = U1RXREG;
    return;
}

/**
 * UART1 char receive function.
 * @return the received character.
 */
char UART1GetChar() {
    U1STAbits.OERR = 0;
    return U1RXREG;
}

/**
 * UART1 char transmit function.
 * @param ch Character to send.
 */
void UART1PutChar(char ch) {
    while (is_busy);
    is_busy = 1;
    U1TXREG = ch;
}

/**
 * UART1 string transmit function.
 * @param string
 */
//void UART1PutString(char *string) {
//    while (*string) {
//        UART1PutChar(*string++); // sends char and points to the next one
//    };
//}

int UART1PutString(char* str) {
    int i;
    int len = strlen(str);

    if (len >= BUFFER_SERIAL_SIZE)
        return -1; // se não cabe no buffer de serial...

    while (is_busy); // espera acabar a última escritura

    for (i = 1; i < len; i++) {
        buffer_serial[buffer_serial_counter] = str[i];
        buffer_serial_counter++;
    }

    is_busy = 1;
    U1TXREG = str[0];

    return 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void) {
    IFS0bits.U1TXIF = 0;

    // continuar apenas se estiver em mode de escrita
    if (!is_busy)
        return;

    // verifica se há algo no buffer
    if (buffer_serial_counter == 0) {
        is_busy = 0;
    } else if (buffer_serial_counter > 0) {
        int i;
        U1TXREG = buffer_serial[0];
        buffer_serial_counter--;

        // escreve o primeiro caractère e desloca todos os outros para a "esquerda"
        // funciona como uma "fila" (FIFO)
        for (i = 0; i < buffer_serial_counter; i++) {
            buffer_serial[i] = buffer_serial[i + 1];
        }
        buffer_serial[buffer_serial_counter] = 0;
    }

    return;
}

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void) {

    flagRX = 1; // Seta a flag serial RX
    IFS0bits.U1RXIF = 0;
}

///**
// * UART1 initiation function.
// */
//void UART1Init() {
//    char trash;
//
//    // set up registers
//    U1BRG = BAUD_RATE_REG;
//    U1MODE = U_MODE;
//    U1STA = U_STA;
//
//    TRISFbits.TRISF4 = 1;   // configure the RX pin (RF4 - RP10)
//    TRISFbits.TRISF5 = 0;   // configure the TX pin (RF5 - RP17)
//
//    _U1RXR = UART1_RX;      // map RX for picDev board
//    UART1_TX = Ro_U1TX;     // map TX for picDev board
//
//    // delete trash
//    while (U1STAbits.URXDA)
//        trash = U1RXREG;
//    return;
//}
//
///**
// * UART1 char receive function.
// * @return returns 1 if there's a character in the buffer, 0 otherwise.
// */
//int UART1Check() {
//    return U1STAbits.URXDA;
//}
//
///**
// * UART1 char receive function.
// * @return the received character.
// */
//char UART1GetChar() {
//    U1STAbits.OERR=0;
//    while (!U1STAbits.URXDA); // wait untill data arrives
//    return U1RXREG;
//}
//
///**
// * UART1 string receive function.
// * @return the received character.
// */
//char* UART1GetString() {
//    char* string = "";
//
//    U1STAbits.OERR=0;
//    while (!U1STAbits.URXDA); // wait untill data arrives
//    while (!U1STAbits.URXDA) {
//        string = string + U1RXREG;
//    }
//    return string;
//}
//
///**
// * UART1 char transmit function.
// * @param ch Character to send.
// */
//void UART1PutChar(char ch) {
//    while (U1STAbits.UTXBF); // transmit ONLY if TX buffer is empty
//    U1TXREG = ch;
//}
//
///**
// * UART1 string transmit function.
// * @param string
// */
//void UART1PutString(char *string) {
//    while (*string) {
//        UART1PutChar(*string++); // sends char and points to the next one
//    };
//}


