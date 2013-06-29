/* 
Copyright 2013 Brad Quick

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <inttypes.h>
#include <compat/twi.h>

#include "lib_i2c.h"

/*************************************************************************
Initialization of the I2C bus interface. Need to be called only once
*************************************************************************/
void lib_i2c_init(void)
   {
   lib_i2c_setclockspeed(I2C_100_MHZ);
   }

void lib_i2c_setclockspeed(unsigned char speed)
   {
   TWSR = 0;                         /* no prescaler */
   if (speed==I2C_100_MHZ) TWBR = ((F_CPU/100000L)-16)/2;  /* must be > 10 for stable operation */
   else if (speed==I2C_400_MHZ) TWBR = ((F_CPU / 400000L) - 16) / 2; // change the I2C clock rate to 400kHz
   }
   
/*************************************************************************   
 Issues a start condition and sends address and transfer direction.
 return 0 = device accessible, 1= failed to access device
*************************************************************************/
unsigned char lib_i2c_start(unsigned char address)
   {
   uint8_t   twst;

   // send START condition
   TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

   // wait until transmission completed
   while(!(TWCR & (1<<TWINT)));

   // check value of TWI Status Register. Mask prescaler bits.
   twst = TW_STATUS & 0xF8;
   if ( (twst != TW_START) && (twst != TW_REP_START)) return 1;

   // send device address
   TWDR = address;
   TWCR = (1<<TWINT) | (1<<TWEN);

   // wail until transmission completed and ACK/NACK has been received
   while(!(TWCR & (1<<TWINT)));

   // check value of TWI Status Register. Mask prescaler bits.
   twst = TW_STATUS & 0xF8;
   if( twst != TW_MT_SLA_ACK) return 2;

   return 0;

   }/* lib_i2c_start */


/*************************************************************************
Issues a start condition and sends address and transfer direction.
If device is busy, use ack polling to wait until device is ready

Input:   address and transfer direction of I2C device
*************************************************************************/
char lib_i2c_start_wait(unsigned char address)
   { // return non zero on error
   uint8_t   twst;
   int count=0;

   while ( 1 )
      {
      if (++count>1000) return(1); // timed out
   
      // send START condition
      TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
   
      // wait until transmission completed
      while(!(TWCR & (1<<TWINT)));
   
      // check value of TWI Status Register. Mask prescaler bits.
      twst = TW_STATUS & 0xF8;
      if ( (twst != TW_START) && (twst != TW_REP_START)) continue;
   
      // send device address
      TWDR = address;
      TWCR = (1<<TWINT) | (1<<TWEN);
   
      // wail until transmission completed
      while(!(TWCR & (1<<TWINT)));
   
      // check value of TWI Status Register. Mask prescaler bits.
      twst = TW_STATUS & 0xF8;
      if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) ) 
         {           
         /* device busy, send stop condition to terminate write operation */
         TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
       
         // wait until stop condition is executed and bus released
         while(TWCR & (1<<TWSTO));
       
         continue;
         }
      //if( twst != TW_MT_SLA_ACK) return 1;
      break;
      }
   return(0);
   }


/*************************************************************************
Issues a repeated start condition and sends address and transfer direction 

Input:   address and transfer direction of I2C device

Return:  0 device accessible
         1 failed to access device
*************************************************************************/
unsigned char lib_i2c_rep_start(unsigned char address)
   {
   return lib_i2c_start( address );

   }


/*************************************************************************
Terminates the data transfer and releases the I2C bus
*************************************************************************/
void lib_i2c_stop(void)
   {
   /* send stop condition */
   TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

   // wait until stop condition is executed and bus released
   while(TWCR & (1<<TWSTO));

   }
   

/*************************************************************************
 Send one byte to I2C device
 
 Input:    byte to be transfered
 Return:   0 write successful 
           1 write failed
*************************************************************************/
unsigned char lib_i2c_write( unsigned char data )
   {   
   uint8_t   twst;

   // send data to the previously addressed device
   TWDR = data;
   TWCR = (1<<TWINT) | (1<<TWEN);

   // wait until transmission completed
   while(!(TWCR & (1<<TWINT)));

   // check value of TWI Status Register. Mask prescaler bits
   twst = TW_STATUS & 0xF8;
   if( twst != TW_MT_DATA_ACK) return 1;
   return 0;

   }


/*************************************************************************
Read one byte from the I2C device, request more data from device 

Return:  byte read from I2C device
*************************************************************************/
unsigned char lib_i2c_readAck(void)
   {
   TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
   while(!(TWCR & (1<<TWINT)));    

   return TWDR;

   }


/*************************************************************************
Read one byte from the I2C device, read is followed by a stop condition 

Return:  byte read from I2C device
*************************************************************************/
unsigned char lib_i2c_readNak(void)
   {
   TWCR = (1<<TWINT) | (1<<TWEN);
   while(!(TWCR & (1<<TWINT)));

   return TWDR;
   }

void lib_i2c_writereg(unsigned char address,unsigned char reg, unsigned char value)
   {
   lib_i2c_start_wait((address<<1)+I2C_WRITE);
   lib_i2c_write(reg);
   lib_i2c_write(value);
   lib_i2c_stop();
   }

unsigned char lib_i2c_readreg(unsigned char address,unsigned char reg)
   {
   lib_i2c_start_wait((address<<1)+I2C_WRITE);
   lib_i2c_write(reg); 
   lib_i2c_rep_start((address<<1)+I2C_READ);

   unsigned char returnvalue = lib_i2c_readNak();

   lib_i2c_stop();
   
   return(returnvalue);
   }

void lib_i2c_readdata(unsigned char address,unsigned char reg,unsigned char *data,unsigned char length)
   {
   lib_i2c_start_wait((address<<1)+I2C_WRITE);
   lib_i2c_write(reg); 
   lib_i2c_rep_start((address<<1)+I2C_READ);

   while (--length)
      {
      *data++ = lib_i2c_readAck();
      }
   *data = lib_i2c_readNak();

   lib_i2c_stop();
   }

