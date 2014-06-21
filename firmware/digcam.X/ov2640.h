/*
 * File:   ov2640.h
 * Author: Fernando Barbosa
 *
 * Created on May 10, 2014, 02:02 AM
 *
 * Description: defines all commands known to the pic. Each command is define by
 * two hexadecimal characters, for a total of 256 commands.
 */

/******************************************************************************/
/* Commands #define Macros                                                    */
/******************************************************************************/

#include "lib/picdev/picDev.h"  // Connections of the picDev board
#include "lib/picdev/pinOut.h"  // Pin mapping of the picDev Board

#include "main.h"

/******************************************************************************/
/* OV2640 Functions                                                           */
/******************************************************************************/

int CamIsOn();
int CamIsReset();

void CamInit();
void CamReset();
void CamTakePic();
void CamTurnOn();
void CamTurnOff();