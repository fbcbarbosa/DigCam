/*
 * File:   delay.c
 * Author: fernando
 *
 * Created on June 9, 2014, 5:33 PM
 */

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <p24Fxxxx.h>

#include "delay.h"
#include "lib/picdev/picDev.h"

/******************************************************************************/
/* User Functions                                                             */

/******************************************************************************/

int count = 0;

/**
 * Delay function.
 * @param t Time in ms
 */
void Delayms( unsigned t)
{
    T1CON = 0x8000;     // enable tmr1, Internal clock (FOSC/2), 1:1
    while (t--)
    {
        TMR1 = 0;
        while (TMR1< GetPeripheralClock()/1000);
    }
}

/**
 * Delay function.
 * @param t Time in us
 */
void Delayus( unsigned t)
{
    T1CON = 0x8000;     // enable tmr1, Internal clock (FOSC/2), 1:1
    while (t--)
    {
        TMR1 = 0;
        while (TMR1< GetPeripheralClock()/1000000);
    }
}