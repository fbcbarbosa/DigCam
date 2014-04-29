//-----------------------------------------------
//              pinOut.h
//      Pinout for the picDev Board
//      Arthur Benemann 27/03/2012
//-----------------------------------------------
//      Description:
//  Pinout for the picDev board, peripherals conenctions, analog pins.
//  Also some macros to help with pin configuration
//-----------------------------------------------

#ifndef _PINOUT
#define _PINOUT
//------------ MACROS --------------------
#define AllPinsDigital() AD1PCFGL = 0xffff
#define OUTPUT 0;
#define INPUT 1;

//------------ I/O -----------------------
#define P1 _RE5
#define P2 _RE6
#define P3 _RE7
#define P4 _RG6
#define P5 _RG7
#define P6 _RG8
#define P8 _RG9
#define P11 _RB5
#define P12 _RB4
#define P13 _RB3
#define P14 _RB2
#define P15 _RB1
#define P16 _RB0
#define P21 _RB8
#define P22 _RB9
#define P23 _RB10
#define P24 _RB11
#define P27 _RB12
#define P28 _RB13
#define P29 _RB14
#define P30 _RB15
#define P31 _RF4
#define P32 _RF5
#define P33 _RF3
#define P42 _RD8
#define P43 _RD9
#define P44 _RD10
#define P45 _RD11
#define P46 _RD0
#define P49 _RD1
#define P50 _RD2
#define P51 _RD3
#define P52 _RD4
#define P53 _RD5
#define P54 _RD6
#define P55 _RD7
#define P58 _RF0
#define P59 _RF1
#define P60 _RE0
#define P61 _RE1
#define P62 _RE2
#define P63 _RE3
#define P64 _RE4

// Tris
#define P1tris  _TRISE5
#define P2tris  _TRISE6
#define P3tris  _TRISE7
#define P4tris  _TRISG6
#define P5tris  _TRISG7
#define P6tris  _TRISG8
#define P8tris  _TRISG9
#define P11tris  _TRISB5
#define P12tris  _TRISB4
#define P13tris  _TRISB3
#define P14tris  _TRISB2
#define P15tris  _TRISB1
#define P16tris  _TRISB0
#define P21tris  _TRISB8
#define P22tris  _TRISB9
#define P23tris  _TRISB10
#define P24tris  _TRISB11
#define P27tris  _TRISB12
#define P28tris  _TRISB13
#define P29tris  _TRISB14
#define P30tris  _TRISB15
#define P31tris  _TRISF4
#define P32tris  _TRISF5
#define P33tris  _TRISF3
#define P42tris  _TRISD8
#define P43tris  _TRISD9
#define P44tris  _TRISD10
#define P45tris  _TRISD11
#define P46tris  _TRISD0
#define P49tris  _TRISD1
#define P50tris  _TRISD2
#define P51tris  _TRISD3
#define P52tris  _TRISD4
#define P53tris  _TRISD5
#define P54tris  _TRISD6
#define P55tris  _TRISD7
#define P58tris  _TRISF0
#define P59tris  _TRISF1
#define P60tris  _TRISE0
#define P61tris  _TRISE1
#define P62tris  _TRISE2
#define P63tris  _TRISE3
#define P64tris  _TRISE4

//------------ Analog pins ---------------
#define A11 _AN5
#define A12 _AN4
#define A13 _AN3
#define A14 _AN2
#define A15 _AN1
#define A16 _AN0
#define A21 _AN8
#define A22 _AN9
#define A23 _AN10
#define A24 _AN11
#define A29 _AN14
#define A30 _AN15



//------------ Remapable pins ------------
// remapable outputs
#define Ro4 _RP21R
#define Ro5 _RP26R
#define Ro6 _RP19R
#define Ro8 _RP27R
#define Ro11 _RP18R
#define Ro12 _RP28R
#define Ro14 _RP13R
#define Ro15 _RP1R
#define Ro16 _RP0R
#define Ro21 _RP8R
#define Ro22 _RP9R
#define Ro29 _RP14R
#define Ro30 _RP29R
#define Ro31 _RP10R
#define Ro32 _RP17R
#define Ro33 _RP16R
#define Ro42 _RP2R
#define Ro43 _RP4R
#define Ro44 _RP3R
#define Ro45 _RP12R
#define Ro46 _RP11R
#define Ro49 _RP24R
#define Ro50 _RP23R
#define Ro51 _RP22R
#define Ro52 _RP25R
#define Ro53 _RP20R
// Remapable inputs
#define Ri4 21
#define Ri5 26
#define Ri6 19
#define Ri8 27
#define Ri11 18
#define Ri12 28
#define Ri14 13
#define Ri15 1
#define Ri16 0
#define Ri21 8
#define Ri22 9
#define Ri29 14
#define Ri30 29
#define Ri31 10
#define Ri32 17
#define Ri33 16
#define Ri42 2
#define Ri43 4
#define Ri44 3
#define Ri45 12
#define Ri46 11
#define Ri49 24
#define Ri50 23
#define Ri51 22
#define Ri52 25
#define Ri53 20
//Remapable peripherals Outputs
#define Ro_NULL     0
#define Ro_C1OUT    1
#define Ro_C2OUT    2
#define Ro_U1TX     3
#define Ro_U1RTS    4
#define Ro_U2TX     5
#define Ro_U2RTS    6
#define Ro_SDO1     7
#define Ro_SCK1OUT  8
#define Ro_SS1OUT   9
#define Ro_SDO2     10
#define Ro_SCK2OUT  11
#define Ro_SS2OUT   12
#define Ro_OC1      18
#define Ro_OC2      19
#define Ro_OC3      20
#define Ro_OC4      21
#define Ro_OC5      22
#define Ro_OC6      23
#define Ro_OC7      24
#define Ro_OC8      25
#define Ro_U3TX     28
#define Ro_U3RTS    29
#define Ro_U4TX     30
#define Ro_U4RTS    31
#define Ro_SDO3     32
#define Ro_SCK3OUT  33
#define Ro_SS3OUT   34
#define Ro_OC9      35
//Remapable peripherals Inputs
#define Ri_INT1     _INT1R
#define Ri_INT2     _INT2R
#define Ri_INT3     _INT3R
#define Ri_INT4     _INT4R
#define Ri_T1CK     _T1CKR
#define Ri_T2CK     _T2CKR
#define Ri_T3CK     _T3CKR
#define Ri_T4CK     _T4CKR
#define Ri_T5CK     _T5CKR
#define Ri_IC1      _IC1R
#define Ri_IC2      _IC2R
#define Ri_IC3      _IC3R
#define Ri_IC4      _IC4R
#define Ri_IC5      _IC5R
#define Ri_IC6      _IC6R
#define Ri_IC7      _IC7R
#define Ri_IC8      _IC8R
#define Ri_IC9      _IC9R
#define Ri_OCFA     _OCFAR
#define Ri_OCFB     _OCFBR
#define Ri_U1RX     _U1RXR
#define Ri_U1CTS    _U1CTSR
#define Ri_U2RX     _U2RXR
#define Ri_U2CTS    _U2CTSR
#define Ri_SDI1     _SDI1R
#define Ri_SCK1IN   _SCK1R
#define Ri_SS1IN    _SS1R
#define Ri_SDI2     _SDI2R
#define Ri_SCK2IN   _SCK2R
#define Ri_SS2IN    _SS2R
#define Ri_U3RX     _U3RXR
#define Ri_U3CTS    _U3CTSR
#define Ri_U4RX     _U4RXR
#define Ri_U4CTS    _U4CTSR
#define Ri_SDI3     _SDI3R
#define Ri_SCK3IN   _SCK3R
#define Ri_SS3IN    _SS3R

#endif
