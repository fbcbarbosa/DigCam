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
#include "delay.h"      // Device header file

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

int count = 0;

/**
 * User Timer1 to create a delay. WARNING: must call 'Timer1Init()' first!
 * @param t Delay in microseconds.
 */
void Delay(int t) {
    count = 0;
    TMR1 = 0;
    while (count < t);
}

void Timer1Init() {
    PR1 = 0x20;        // 1 us delay for a 32 MHz clock
    IPC0bits.T1IP = 5; // set interrupt priority
    T1CON = 0b1000000000000000; // turn on the timer
    IFS0bits.T1IF = 0; // reset interrupt flag
    IEC0bits.T1IE = 1; // turn on the timer1 interrupt
}

void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void) {
    count++;
    IFS0bits.T1IF = 0;
}