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

#include "projectsettings.h"

#define DIGITALINPUT 0
#define DIGITALOUTPUT 1

#define PULLUPRESISTOR 0
#define NOPULLUPRESISTOR 2

#ifdef USEDIGITALPORTB
   #define DIGITALPORTB 0x10
#endif
#ifdef USEDIGITALPORTC
   #define DIGITALPORTC 0x20
#endif
#ifdef USEDIGITALPORTD
   #define DIGITALPORTD 0x30
#endif
#ifdef USEDIGITALPORTE
   #define DIGITALPORTE 0x40
#endif
#ifdef USEDIGITALPORTF
   #define DIGITALPORTF 0x50
#endif
#ifdef USEDIGITALPORTG
   #define DIGITALPORTG 0x60
#endif
#ifdef USEDIGITALPORTH
   #define DIGITALPORTH 0x70
#endif
#ifdef USEDIGITALPORTI
   #define DIGITALPORTI 0x80
#endif
#ifdef USEDIGITALPORTJ
   #define DIGITALPORTJ 0x90
#endif
#ifdef USEDIGITALPORTK
   #define DIGITALPORTK 0xA0
#endif
#ifdef USEDIGITALPORTL
   #define DIGITALPORTL 0xB0
#endif

#define DIGITALHIGH 0
#define DIGITALLOW 1

#define DIGITALON 1
#define DIGITALOFF 0

#define PCINTERRUPT 0
#define INTERRUPT 0x40

void lib_digitalio_initpin(unsigned char portandpinnumber,unsigned char output);
unsigned char lib_digitalio_getinput(unsigned char portandpinnumber);
void lib_digitalio_setoutput(unsigned char portandpinnumber,unsigned char value);

typedef void (* digitalcallbackfunctptr)(unsigned char interruptnumber,unsigned char newpinstate);
void lib_digitalio_setinterruptcallback(unsigned char portandpinnumber,digitalcallbackfunctptr callback);
