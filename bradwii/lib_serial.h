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

void lib_serial_initport(unsigned char serialportnumber,long baud);
void lib_serial_sendchar(unsigned char serialportnumber,unsigned char c);
void lib_serial_sendstring(unsigned char serialportnumber,char *string);
void lib_serial_senddata(unsigned char serialportnumber,unsigned char *data,int datalength);
int lib_serial_numcharsavailable(unsigned char serialportnumber);
unsigned char lib_serial_getchar(unsigned char serialportnumber);
void lib_serial_getdata(unsigned char serialportnumber,unsigned char *data,int datalength);
int lib_serial_availableoutputbuffersize(unsigned char serialportnumber);

typedef void (* serialcallbackfunctptr)(unsigned char c);
void lib_serial_setrxcallback(unsigned char serialportnumber,serialcallbackfunctptr callback);


#define USBPORTNUMBER 5