//-----------------------------------------------
//              ConfigurationBits.c
//      Configuration Bits for the picDev Board.
//      Arthur Benemann 27/03/2012
//-----------------------------------------------
//      Description:
//  Configuration bits for the pic24fj256gb106 on the picDev board.
//  A simple but powerfull configuration.
//-----------------------------------------------

#include <p24Fxxxx.h>       // Compiler Definitions

//-------------------- CONFIGURATION --------------------
// Disables memomory protection
_CONFIG3(WPDIS_WPDIS & WPCFG_WPCFGDIS);
// Primary oscillator with PLL, High Speed OSC, Usb vreg on, Clock switching enabled, Two speed start-up enabled, PLL with a 12MHz OSC
_CONFIG2(FNOSC_PRIPLL  & POSCMOD_HS & DISUVREG_ON & IOL1WAY_OFF  & FCKSM_CSECMD & IESO_ON & PLLDIV_DIV3);
// Use PGD2 and PGC2 pins for debbuging, Disable wathchdog, Disable JTAG
_CONFIG1(ICS_PGx2 & FWDTEN_OFF & JTAGEN_OFF);
