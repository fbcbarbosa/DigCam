/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 */

/* $Id: twitest.c,v 1.1 2002/12/18 22:35:38 joerg_wunsch Exp $ */

/*
 * Simple demo program that talks to a 24Cxx I²C EEPROM using the
 * builtin TWI interface of an ATmega device.
 */

/*
 * ---------------------------------------------------------------------------
 * The code modified and reorganized by Jaakko Ala-Paavola 2003/08/20
 * jap@iki.fi
 * ---------------------------------------------------------------------------
 */

void reset_i2c_bus(void);

#include "i2c_cam.h"
#include <i2c.h>

//function initiates I2C1 module to baud rate BRG
void i2c_init(int BRG)
{
   int temp;

   // I2CBRG = 194 for 10Mhz OSCI with PPL with 100kHz I2C clock
   I2C1BRG = BRG;
   I2C1CONbits.I2CEN = 0;	// Disable I2C Mode
   I2C1CONbits.DISSLW = 1;	// Disable slew rate control
   IFS1bits.MI2C1IF = 0;	// Clear Interrupt
   I2C1CONbits.I2CEN = 1;	// Enable I2C Mode
   //temp = I2CRCV;               // read buffer to clear buffer full
   reset_i2c_bus();             // set bus to idle
}

//Resets the I2C bus to Idle
void reset_i2c_bus(void)
{
   int x = 0;

   //initiate stop bit
   I2C1CONbits.PEN = 1;

   //wait for hardware clear of stop bit
   while (I2C1CONbits.PEN)
   {
   //   DelayuSec(1);
      x++;
      if (x > 20) break;
   }
   I2C1CONbits.RCEN = 0;
   IFS1bits.MI2C1IF = 0; // Clear Interrupt
   I2C1STATbits.IWCOL = 0;
   I2C1STATbits.BCL = 0;
   //DelayuSec(10);
}

//void
//i2c_init(int clk)
//{
//
//  /* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
//#if defined(TWPS0)
//  /* has prescaler (mega128 & newer) */
//  TWSR = 0;
//#endif
//  TWBR = (clk*1000000 / 100000UL - 16) / 2;
//
//}
//
//int
//i2c_read_bytes(uint16_t eeaddr, int len, uint8_t *buf)
//{
//
//  unsigned long int counter=0;
//  uint8_t sla, twcr, n = 0;
//  int rv = 0;
//
//
//  /* patch high bits of EEPROM address into SLA */
//  sla = TWI_SLA_CAM |(((eeaddr >> 8) & 0x07) << 1);
//
//  /*
//   * Note [6]
//   * First cycle: master transmitter mode
//   */
//  restart:
//  if (n++ >= MAX_ITER)
//    return -1;
//  begin:
//
//  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); /* send start condition */
//  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
//  switch ((TW_STATUS))
//    {
//    case TW_REP_START:          /* OK, but should not happen */
//    case TW_START:
//      break;
//
//    case TW_MT_ARB_LOST:        /* Note [7] */
//      goto begin;
//
//    default:
//      return -1;                /* error: not in start condition */
//                                /* NB: do /not/ send stop condition */
//    }
//
//
//   /* Note [8] */
//  /* send SLA+W */
//  TWDR = sla | TW_WRITE;
//  TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
//  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
//  switch ((TW_STATUS))
//    {
//    case TW_MT_SLA_ACK:
//      break;
//
//    case TW_MT_SLA_NACK:        /* nack during select: device busy writing */
//                                /* Note [9] */
//      goto restart;
//
//    case TW_MT_ARB_LOST:        /* re-arbitrate */
//      goto begin;
//
//    default:
//      goto error;               /* must send stop condition */
//    }
//
//  TWDR = eeaddr;                /* low 8 bits of addr */
//  TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
//  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
//  switch ((TW_STATUS))
//    {
//    case TW_MT_DATA_ACK:
//      break;
//
//    case TW_MT_DATA_NACK:
//      goto quit;
//
//    case TW_MT_ARB_LOST:
//      goto begin;
//
//    default:
//      goto error;               /* must send stop condition */
//    }
//
//
//
//  TWCR = 0; // Stop the twi interface to make the camera able to rescognise the new start
//  while (counter != 0x0020)
//		{
//		    counter++;
//		}
//
//
//  /*
//   * Note [10]
//   * Next cycle(s): master receiver mode
//   */
//  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); /* send (rep.) start condition */
//  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
//  switch ((TW_STATUS))
//    {
//    case TW_START:              /* OK, but should not happen */
//    case TW_REP_START:
//      break;
//
//    case TW_MT_ARB_LOST:
//      goto begin;
//
//    default:
//      goto error;
//    }
//
//  /* send SLA+R */
//  TWDR = (sla | TW_READ);
//  TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
//  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
//  switch ((TW_STATUS))
//    {
//    case TW_MR_SLA_ACK:
//      break;
//
//    case TW_MR_SLA_NACK:
//      goto quit;
//
//    case TW_MR_ARB_LOST:
//      goto begin;
//
//    default:
//      goto error;
//    }
//
//  for (twcr = _BV(TWINT) | _BV(TWEN) | _BV(TWEA) /* Note [11] */;
//       len > 0;
//       len--)
//    {
//      if (len == 1)
//        twcr = _BV(TWINT) | _BV(TWEN); /* send NAK this time */
//      TWCR = twcr;              /* clear int to start transmission */
//      while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
//      switch ((TW_STATUS))
//        {
//        case TW_MR_DATA_NACK:
//          len = 0;              /* force end of loop */
//          /* FALLTHROUGH */
//        case TW_MR_DATA_ACK:
//          *buf++ = TWDR;
//          rv++;
//          break;
//
//        default:
//          goto error;
//        }
//    }
//  quit:
//  /* Note [12] */
//  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); /* send stop condition */
//
//  return rv;
//
//  error:
//  rv = -1;
//  goto quit;
//}
//
//int
//i2c_write_page(uint16_t eeaddr, int len, uint8_t *buf)
//{
//  uint8_t sla, n = 0;
//  int rv = 0;
//  uint16_t endaddr;
//
//  if (eeaddr + len < (eeaddr | (PAGE_SIZE - 1)))
//    endaddr = eeaddr + len;
//  else
//    endaddr = (eeaddr | (PAGE_SIZE - 1)) + 1;
//  len = endaddr - eeaddr;
//
//  /* patch high bits of EEPROM address into SLA */
//  sla = TWI_SLA_CAM | (((eeaddr >> 8) & 0x07) << 1);
//
//  restart:
//  if (n++ >= MAX_ITER)
//    return -1;
//  begin:
//
//  /* Note 13 */
//  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); /* send start condition */
//  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
//  switch ((TW_STATUS))
//    {
//    case TW_REP_START:          /* OK, but should not happen */
//    case TW_START:
//      break;
//
//    case TW_MT_ARB_LOST:
//      goto begin;
//
//    default:
//      return -1;                /* error: not in start condition */
//                                /* NB: do /not/ send stop condition */
//    }
//
//  /* send SLA+W */
//  TWDR = sla | TW_WRITE;
//  TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
//  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
//  switch ((TW_STATUS))
//    {
//    case TW_MT_SLA_ACK:
//      break;
//
//    case TW_MT_SLA_NACK:        /* nack during select: device busy writing */
//      goto restart;
//
//    case TW_MT_ARB_LOST:        /* re-arbitrate */
//      goto begin;
//
//    default:
//      goto error;               /* must send stop condition */
//    }
//
//  TWDR = eeaddr;                /* low 8 bits of addr */
//  TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
//  while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
//  switch ((TW_STATUS))
//    {
//    case TW_MT_DATA_ACK:
//      break;
//
//    case TW_MT_DATA_NACK:
//      goto quit;
//
//    case TW_MT_ARB_LOST:
//      goto begin;
//
//    default:
//      goto error;               /* must send stop condition */
//    }
//
//  for (; len > 0; len--)
//    {
//      TWDR = *buf++;
//      TWCR = _BV(TWINT) | _BV(TWEN); /* start transmission */
//      while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
//      switch ((TW_STATUS))
//        {
//        case TW_MT_DATA_NACK:
//          goto error;           /* device write protected -- Note [14] */
//
//        case TW_MT_DATA_ACK:
//          rv++;
//          break;
//
//        default:
//          goto error;
//        }
//    }
//  quit:
//  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); /* send stop condition */
//
//  return rv;
//
//  error:
//  rv = -1;
//  goto quit;
//}
//
//int
//i2c_write_bytes(uint16_t eeaddr, int len, uint8_t *buf)
//{
//  int rv, total;
//
//  total = 0;
//  do
//    {
//#if DEBUG
//      printf("Calling i2c_write_page(%d, %d, %p)",
//             eeaddr, len, buf);
//#endif
//      rv = i2c_write_page(eeaddr, len, buf);
//#if DEBUG
//      printf(" => %d\n", rv);
//#endif
//      if (rv == -1)
//        return -1;
//      eeaddr += rv;
//      len -= rv;
//      buf += rv;
//      total += rv;
//    }
//  while (len > 0);
//
//  return total;
//}