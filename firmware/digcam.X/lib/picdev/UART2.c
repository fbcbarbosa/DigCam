//------------------------------------------------
//		UART2.c
//	Serial communication library
//	Arthur Benemann 13/11/2010
//------------------------------------------------
//	Description:
//	Open UART peripheral for serial communication.
//  Contain some basic input/output rotines
//-----------------------------------------------
//	Requeriments:
//	UART2
//	2 Remapable pin (RX,TX)
//-----------------------------------------------

#include "UART2.h"
#include "picDev.h"

//-------------- Configuração dos perifericos -------------------
#if defined(U2_INV)
#define	U_MODE		0x8018	// Enable UART, BREGH=1, 8n1
#define	U_STA		0x4400	// Ativa transmissão. limpa flags
#else
#define	U_MODE		0x8008	// Enable UART, BREGH=1, 8n1
#define	U_STA		0x0400	// Ativa transmissão. limpa flags
#endif

#ifndef BRATE2
#define BRATE2 115200
#warning 'Baudrate for the UART2 not set using 115200 as default'
#warning 'please define it using the folowing command: #define BRATE2 115200'
#endif

//------------------------ BAUDRATE -----------------------------
#define BAUDRATEREG    (((GetPeripheralClock())-(4*BRATE2))/(4*BRATE2))
#define BAUD_ACTUAL		((GetPeripheralClock())	/(4*(BAUDRATEREG+1)))

#define BAUD_ERROR              ((BAUD_ACTUAL > BRATE2) ? BAUD_ACTUAL-BRATE2 : BRATE2-BAUD_ACTUAL)
#define BAUD_ERROR_PERCENT      ((BAUD_ERROR*100+BRATE2/2)/BRATE2)

#if (BAUD_ERROR_PERCENT > 3)
#error UART frequency error is worse than 3%
#elif (BAUD_ERROR_PERCENT > 2)
#warning UART frequency error is worse than 2%
#endif


//-----------------Public Functions ------------------------------

void initU2(void) {
    char lixo;

    _U2RXIE = 0;
    U2BRG = BAUDRATEREG; // Inicializações
    U2MODE = U_MODE;
    U2STA = U_STA;


    Ri_U2RX = UART2_RX;
    UART2_TX = Ro_U2TX;

    while (U2STAbits.URXDA)
        lixo = U2RXREG;

#ifdef USE_SERIAL_INT
    _U2RXIE = 1;
#endif

} //initU2

void putU2(int c) {
    while (U2STAbits.UTXBF); // Espera o buffer de saidar ficar vazio
    U2TXREG = c; // Transmite o dado
} //putU2

char checkU2(void) {
    return U2STAbits.URXDA;
}

char getU2(void) {
    U2STAbits.OERR=0;
    while (!U2STAbits.URXDA); // Espera até receber um dado
    return U2RXREG; // Retorna o dado recebido
}

void putsU2(char *s){ // envia uma string
    while (*s) // loop até *s == '\0',final da string
        putU2(*s++); // Envia o caracter a aponta para o proximo
}

char* appendHexaInt(char *str, unsigned int data) {
    unsigned char hexa;
    *str++ = 'x';
    hexa = (data >> 12) &0x0f;
    *str++ = ((hexa > 9u) ? hexa + 'A' - 10 : hexa + '0');
    hexa = (data >> 8) & 0x0f;
    *str++ = ((hexa > 9u) ? hexa + 'A' - 10 : hexa + '0');
    hexa = (data >> 4) & 0x0f;
    *str++ = ((hexa > 9u) ? hexa + 'A' - 10 : hexa + '0');
    hexa = (data >> 0) & 0x0f;
    *str++ = ((hexa > 9u) ? hexa + 'A' - 10 : hexa + '0');
    return str;
}
