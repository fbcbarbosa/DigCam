/*
 * ----------------------------------------------------------------------------
 * BMP Lybrary
 *
 * By Iñaki Navarro Oiza
 * April 2004
 * ----------------------------------------------------------------------------
 */
 
void senddata(void);
void sendtable(void);
void createheader(char *header, int heigh, int width);
void createinfoheader(char *infoheader, int heigh, int width);
void usart_putnumchars(char *header, int num);
