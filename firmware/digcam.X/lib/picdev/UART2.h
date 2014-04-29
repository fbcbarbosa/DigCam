//------------------------------------------------
//		UART2.h
//	Serial communication library
//	Arthur Benemann 13/11/2010
//------------------------------------------------
//
//	Description:
//	Open UART peripheral for serial communication.
//  Contain some basic input/output rotines
//
//-----------------------------------------------
//
//	Requeriments:
//	UART2
//	2 Remapable pin (RX,TX)
//
//-----------------------------------------------

//----------------- Macros ---------------------
#define lineU2()		putU2('\r');putU2('\n')

//---------------- Functions ---------------------
void initU2(void);	 	// inicializa a porta seria (8n1, UART2)
void putU2(int c);	 	// envia um caracter para a porta
void putsU2(char *s); 	// envia uma string terminada em null
char getU2(void);	 	// espera até receber um caracter da porta
char checkU2(void);
char* appendHexaInt(char *str, unsigned int data);
