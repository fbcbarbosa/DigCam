/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

#define write(s)            UART1PutString((char*)s)
#define writeln(s)          UART1PutString((char*)s);UART1PutString("\r\n")
#define writech(ch)         UART1PutChar((char)ch)
#define read()              UART1GetChar()

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);                 // I/O and Peripheral Initialization
void RepeaterProcessEvents(void);   // Repeater main function