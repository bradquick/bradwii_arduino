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

#pragma once

#define    I2C_READ   1
#define    I2C_WRITE   0

#define I2C_100_KHZ 0
#define I2C_400_KHZ 1

void lib_i2c_init(void);
void lib_i2c_setclockspeed(unsigned char speed);
unsigned char lib_i2c_start(unsigned char address);
char lib_i2c_start_wait(unsigned char address);
unsigned char lib_i2c_rep_start(unsigned char address);
void lib_i2c_stop(void);
unsigned char lib_i2c_write( unsigned char data );
unsigned char lib_i2c_readack(void);
unsigned char lib_i2c_readnak(void);
void lib_i2c_writereg(unsigned char address,unsigned char reg, unsigned char value);
unsigned char lib_i2c_readreg(unsigned char address,unsigned char reg);
void lib_i2c_readdata(unsigned char address,unsigned char register,unsigned char *data,unsigned char length);

