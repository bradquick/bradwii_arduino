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

#include <avr/io.h>
#include <avr/interrupt.h>
#include "lib_digitalio.h"
#include "projectsettings.h"

// This code controls the simple digital IO

// Usage:

// #define LEDOUTPUT (DIGITALPORTB | 7)     // defines the reference for pin 7 on PORTB
// lib_digitalio_initpin(LEDOUTPUT,DIGITALOUTPUT | PULLUPRESISTOR); // set the pin as an output (also turns on the pull up resistor)
// lib_digitalio_setoutput(LEDOUTPUT, DIGITALLOW); // turn the output on by pulling it low

// #define PUSHBUTTON (DIGITALPORTB | 4)     // defines the reference for pin 4 on PORTB
// #define INTERRUPT6PORTANDPIN PUSHBUTTON
// lib_digitalio_initpin(PUSHBUTTON,DIGITALINPUT);  // set the pin as an input (also turns on the pull up resistor)
// if (lib_digitalio_getinput(PUSHBUTTON)) {}       // Check the input
// lib_digitalio_setinterruptcallback(PUSHBUTTON,mypushbuttoncallback); // tell the interrupt
// void mypushbuttoncallback(char interruptnumber,char newstate) // call back will get called any time the pin changes
//      {
//      if (newstate==DIGITALON) {}
//      }


// ports to be used as digital IO need to be defined in projectsettings.h
#define SETUPDIGITALIOMACRO(myport,avrport,avrddr,pinnumber) \
   if (port==myport) \
      { \
      if (output & DIGITALOUTPUT)\
         { \
         if (!(output & NOPULLUPRESISTOR)) avrport |= (1 << pinnumber); /* pull up resistor on */\ 
         avrddr |= (1 << pinnumber); /* this is an output */ \
         }\
      else\
         {\
         avrddr &= ~(1 << pinnumber); /* set it to an input*/\
         if (!(output & NOPULLUPRESISTOR)) avrport |= (1 << pinnumber); /* pull up resistor on*/\
         }\
      }\

void lib_digitalio_initpin(unsigned char pinnumber,unsigned char output)
   { // set pin pinnumber to be an output if output==1, othewise set it to be an input
   unsigned char port=pinnumber & 0xf0;
   pinnumber &= 0x0f;

#ifdef USEDIGITALPORTB
   SETUPDIGITALIOMACRO(DIGITALPORTB,PORTB,DDRB,pinnumber);
#endif
#ifdef USEDIGITALPORTC
   SETUPDIGITALIOMACRO(DIGITALPORTC,PORTC,DDRC,pinnumber);
#endif
#ifdef USEDIGITALPORTD
   SETUPDIGITALIOMACRO(DIGITALPORTD,PORTD,DDRD,pinnumber);
#endif
#ifdef USEDIGITALPORTE
   SETUPDIGITALIOMACRO(DIGITALPORTE,PORTE,DDRE,pinnumber);
#endif
#ifdef USEDIGITALPORTF
   SETUPDIGITALIOMACRO(DIGITALPORTF,PORTF,DDRF,pinnumber);
#endif
#ifdef USEDIGITALPORTG
   SETUPDIGITALIOMACRO(DIGITALPORTG,PORTG,DDRG,pinnumber);
#endif
#ifdef USEDIGITALPORTH
   SETUPDIGITALIOMACRO(DIGITALPORTH,PORTH,DDRH,pinnumber);
#endif
#ifdef USEDIGITALPORTI
   SETUPDIGITALIOMACRO(DIGITALPORTI,PORTI,DDRI,pinnumber);
#endif
#ifdef USEDIGITALPORTI
   SETUPDIGITALIOMACRO(DIGITALPORTJ,PORTJ,DDRJ,pinnumber);
#endif
#ifdef USEDIGITALPORTK
   SETUPDIGITALIOMACRO(DIGITALPORTK,PORTK,DDRK,pinnumber);
#endif
#ifdef USEDIGITALPORTL
   SETUPDIGITALIOMACRO(DIGITALPORTL,PORTL,DDRL,pinnumber);
#endif

   }

#define GETDIGITALINPUTMACRO(myport,avrpin,pinnumber) \
   if (port==myport) \
      {\
      if (avrpin & (1 << pinnumber)) return(0);\
      else return(1);\
      }\
   

unsigned char lib_digitalio_getinput(unsigned char pinnumber)
   {
   unsigned char port=pinnumber & 0xf0;
   pinnumber &= 0x0f;

#ifdef USEDIGITALPORTB
   GETDIGITALINPUTMACRO(DIGITALPORTB,PINB,pinnumber);
#endif
#ifdef USEDIGITALPORTC
   GETDIGITALINPUTMACRO(DIGITALPORTC,PINC,pinnumber);
#endif
#ifdef USEDIGITALPORTD
   GETDIGITALINPUTMACRO(DIGITALPORTD,PIND,pinnumber);
#endif
#ifdef USEDIGITALPORTE
   GETDIGITALINPUTMACRO(DIGITALPORTE,PINE,pinnumber);
#endif
#ifdef USEDIGITALPORTF
   GETDIGITALINPUTMACRO(DIGITALPORTF,PINF,pinnumber);
#endif
#ifdef USEDIGITALPORTG
   GETDIGITALINPUTMACRO(DIGITALPORTG,PING,pinnumber);
#endif
#ifdef USEDIGITALPORTH
   GETDIGITALINPUTMACRO(DIGITALPORTH,PINH,pinnumber);
#endif
#ifdef USEDIGITALPORTI
   GETDIGITALINPUTMACRO(DIGITALPORTI,PINI,pinnumber);
#endif
#ifdef USEDIGITALPORTJ
   GETDIGITALINPUTMACRO(DIGITALPORTJ,PINJ,pinnumber);
#endif
#ifdef USEDIGITALPORTK
   GETDIGITALINPUTMACRO(DIGITALPORTK,PINK,pinnumber);
#endif
#ifdef USEDIGITALPORTL
   GETDIGITALINPUTMACRO(DIGITALPORTL,PINL,pinnumber);
#endif
   }

#define SETDIGITALOUTPUTMACRO(value,myport,avrport,pinnumber) \
   if (port==myport) \
      {\
      if (!value) avrport |= (1 << pinnumber);\
      else avrport &= ~(1 << pinnumber);\
      }\
      
void lib_digitalio_setoutput(unsigned char pinnumber,unsigned char value)
   {
   unsigned char port=pinnumber & 0xf0;
   pinnumber &= 0x0f;

#ifdef USEDIGITALPORTB
   SETDIGITALOUTPUTMACRO(value,DIGITALPORTB,PORTB,pinnumber);
#endif
#ifdef USEDIGITALPORTC
   SETDIGITALOUTPUTMACRO(value,DIGITALPORTC,PORTC,pinnumber);
#endif
#ifdef USEDIGITALPORTD
   SETDIGITALOUTPUTMACRO(value,DIGITALPORTD,PORTD,pinnumber);
#endif
#ifdef USEDIGITALPORTE
   SETDIGITALOUTPUTMACRO(value,DIGITALPORTE,PORTE,pinnumber);
#endif
#ifdef USEDIGITALPORTF
   SETDIGITALOUTPUTMACRO(value,DIGITALPORTF,PORTF,pinnumber);
#endif
#ifdef USEDIGITALPORTG
   SETDIGITALOUTPUTMACRO(value,DIGITALPORTG,PORTG,pinnumber);
#endif
#ifdef USEDIGITALPORTH
   SETDIGITALOUTPUTMACRO(value,DIGITALPORTH,PORTH,pinnumber);
#endif
#ifdef USEDIGITALPORTI
   SETDIGITALOUTPUTMACRO(value,DIGITALPORTI,PORTI,pinnumber);
#endif
#ifdef USEDIGITALPORTJ
   SETDIGITALOUTPUTMACRO(value,DIGITALPORTJ,PORTJ,pinnumber);
#endif
#ifdef USEDIGITALPORTK
   SETDIGITALOUTPUTMACRO(value,DIGITALPORTK,PORTK,pinnumber);
#endif
#ifdef USEDIGITALPORTL
   SETDIGITALOUTPUTMACRO(value,DIGITALPORTL,PORTL,pinnumber);
#endif
   }


#define SETUPINTMACRO(INTERRUPTxPORTANDPIN,INTERRUPTxPORT,digitalcallbackx,INTx_vect) \
digitalcallbackfunctptr digitalcallbackx=0; \
ISR (INTx_vect) \
   { \
   unsigned char state=(INTERRUPTxPORT & (1<<(INTERRUPTxPORTANDPIN & 0x0F)))==0; \
   sei(); \
    \
   if (digitalcallbackx) (digitalcallbackx)(INTERRUPTxPORTANDPIN,state); \
   }


#ifdef INTERRUPT0PORTANDPIN
   #if ((INTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTB) 
      #define INTERRUPT0PORT PINB 
   #elif ((INTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTC) 
      #define INTERRUPT0PORT PINC 
   #elif ((INTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTD) 
      #define INTERRUPT0PORT PIND 
   #elif ((INTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTE) 
      #define INTERRUPT0PORT PINE 
   #elif ((INTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTF) 
      #define INTERRUPT0PORT PINF 
   #elif ((INTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTG) 
      #define INTERRUPT0PORT PING 
   #elif ((INTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTH) 
      #define INTERRUPT0PORT PINH 
   #elif ((INTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTI) 
      #define INTERRUPT0PORT PINI 
   #elif ((INTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTJ) 
      #define INTERRUPT0PORT PINJ 
   #elif ((INTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTK) 
      #define INTERRUPT0PORT PINK 
   #elif ((INTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTL) 
      #define INTERRUPT0PORT PINL 
   #endif
   SETUPINTMACRO(INTERRUPT0PORTANDPIN,INTERRUPT0PORT,digitalcallback0,INT0_vect)
#endif
#ifdef INTERRUPT1PORTANDPIN
   #if ((INTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTB) 
      #define INTERRUPT1PORT PINB 
   #elif ((INTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTC) 
      #define INTERRUPT1PORT PINC 
   #elif ((INTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTD) 
      #define INTERRUPT1PORT PIND 
   #elif ((INTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTE) 
      #define INTERRUPT1PORT PINE 
   #elif ((INTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTF) 
      #define INTERRUPT1PORT PINF 
   #elif ((INTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTG) 
      #define INTERRUPT1PORT PING 
   #elif ((INTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTH) 
      #define INTERRUPT1PORT PINH 
   #elif ((INTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTI) 
      #define INTERRUPT1PORT PINI 
   #elif ((INTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTJ) 
      #define INTERRUPT1PORT PINJ 
   #elif ((INTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTK) 
      #define INTERRUPT1PORT PINK 
   #elif ((INTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTL) 
      #define INTERRUPT1PORT PINL 
   #endif
   SETUPINTMACRO(INTERRUPT1PORTANDPIN,INTERRUPT1PORT,digitalcallback1,INT1_vect)
#endif
#ifdef INTERRUPT2PORTANDPIN
   #if ((INTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTB) 
      #define INTERRUPT2PORT PINB 
   #elif ((INTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTC) 
      #define INTERRUPT2PORT PINC 
   #elif ((INTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTD) 
      #define INTERRUPT2PORT PIND 
   #elif ((INTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTE) 
      #define INTERRUPT2PORT PINE 
   #elif ((INTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTF) 
      #define INTERRUPT2PORT PINF 
   #elif ((INTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTG) 
      #define INTERRUPT2PORT PING 
   #elif ((INTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTH) 
      #define INTERRUPT2PORT PINH 
   #elif ((INTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTI) 
      #define INTERRUPT2PORT PINI 
   #elif ((INTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTJ) 
      #define INTERRUPT2PORT PINJ 
   #elif ((INTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTK) 
      #define INTERRUPT2PORT PINK 
   #elif ((INTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTL) 
      #define INTERRUPT2PORT PINL 
   #endif
   SETUPINTMACRO(INTERRUPT2PORTANDPIN,INTERRUPT2PORT,digitalcallback2,INT2_vect)
#endif
#ifdef INTERRUPT3PORTANDPIN
   #if ((INTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTB) 
      #define INTERRUPT3PORT PINB 
   #elif ((INTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTC) 
      #define INTERRUPT3PORT PINC 
   #elif ((INTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTD) 
      #define INTERRUPT3PORT PIND 
   #elif ((INTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTE) 
      #define INTERRUPT3PORT PINE 
   #elif ((INTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTF) 
      #define INTERRUPT3PORT PINF 
   #elif ((INTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTG) 
      #define INTERRUPT3PORT PING 
   #elif ((INTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTH) 
      #define INTERRUPT3PORT PINH 
   #elif ((INTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTI) 
      #define INTERRUPT3PORT PINI 
   #elif ((INTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTJ) 
      #define INTERRUPT3PORT PINJ 
   #elif ((INTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTK) 
      #define INTERRUPT3PORT PINK 
   #elif ((INTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTL) 
      #define INTERRUPT3PORT PINL 
   #endif
   SETUPINTMACRO(INTERRUPT3PORTANDPIN,INTERRUPT3PORT,digitalcallback3,INT3_vect)
#endif
#ifdef INTERRUPT4PORTANDPIN
   #if ((INTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTB) 
      #define INTERRUPT4PORT PINB 
   #elif ((INTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTC) 
      #define INTERRUPT4PORT PINC 
   #elif ((INTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTD) 
      #define INTERRUPT4PORT PIND 
   #elif ((INTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTE) 
      #define INTERRUPT4PORT PINE 
   #elif ((INTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTF) 
      #define INTERRUPT4PORT PINF 
   #elif ((INTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTG) 
      #define INTERRUPT4PORT PING 
   #elif ((INTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTH) 
      #define INTERRUPT4PORT PINH 
   #elif ((INTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTI) 
      #define INTERRUPT4PORT PINI 
   #elif ((INTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTJ) 
      #define INTERRUPT4PORT PINJ 
   #elif ((INTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTK) 
      #define INTERRUPT4PORT PINK 
   #elif ((INTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTL) 
      #define INTERRUPT4PORT PINL 
   #endif
   SETUPINTMACRO(INTERRUPT4PORTANDPIN,INTERRUPT4PORT,digitalcallback4,INT4_vect)
#endif
#ifdef INTERRUPT5PORTANDPIN
   #if ((INTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTB) 
      #define INTERRUPT5PORT PINB 
   #elif ((INTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTC) 
      #define INTERRUPT5PORT PINC 
   #elif ((INTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTD) 
      #define INTERRUPT5PORT PIND 
   #elif ((INTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTE) 
      #define INTERRUPT5PORT PINE 
   #elif ((INTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTF) 
      #define INTERRUPT5PORT PINF 
   #elif ((INTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTG) 
      #define INTERRUPT5PORT PING 
   #elif ((INTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTH) 
      #define INTERRUPT5PORT PINH 
   #elif ((INTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTI) 
      #define INTERRUPT5PORT PINI 
   #elif ((INTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTJ) 
      #define INTERRUPT5PORT PINJ 
   #elif ((INTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTK) 
      #define INTERRUPT5PORT PINK 
   #elif ((INTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTL) 
      #define INTERRUPT5PORT PINL 
   #endif
   SETUPINTMACRO(INTERRUPT5PORTANDPIN,INTERRUPT5PORT,digitalcallback5,INT5_vect)
#endif
#ifdef INTERRUPT6PORTANDPIN
   #if ((INTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTB) 
      #define INTERRUPT6PORT PINB 
   #elif ((INTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTC) 
      #define INTERRUPT6PORT PINC 
   #elif ((INTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTD) 
      #define INTERRUPT6PORT PIND 
   #elif ((INTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTE) 
      #define INTERRUPT6PORT PINE 
   #elif ((INTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTF) 
      #define INTERRUPT6PORT PINF 
   #elif ((INTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTG) 
      #define INTERRUPT6PORT PING 
   #elif ((INTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTH) 
      #define INTERRUPT6PORT PINH 
   #elif ((INTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTI) 
      #define INTERRUPT6PORT PINI 
   #elif ((INTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTJ) 
      #define INTERRUPT6PORT PINJ 
   #elif ((INTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTK) 
      #define INTERRUPT6PORT PINK 
   #elif ((INTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTL) 
      #define INTERRUPT6PORT PINL 
   #endif
   SETUPINTMACRO(INTERRUPT6PORTANDPIN,INTERRUPT6PORT,digitalcallback6,INT6_vect)
#endif
#ifdef INTERRUPT7PORTANDPIN
   #if ((INTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTB) 
      #define INTERRUPT7PORT PINB 
   #elif ((INTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTC) 
      #define INTERRUPT7PORT PINC 
   #elif ((INTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTD) 
      #define INTERRUPT7PORT PIND 
   #elif ((INTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTE) 
      #define INTERRUPT7PORT PINE 
   #elif ((INTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTF) 
      #define INTERRUPT7PORT PINF 
   #elif ((INTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTG) 
      #define INTERRUPT7PORT PING 
   #elif ((INTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTH) 
      #define INTERRUPT7PORT PINH 
   #elif ((INTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTI) 
      #define INTERRUPT7PORT PINI 
   #elif ((INTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTJ) 
      #define INTERRUPT7PORT PINJ 
   #elif ((INTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTK) 
      #define INTERRUPT7PORT PINK 
   #elif ((INTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTL) 
      #define INTERRUPT7PORT PINL 
   #endif
   SETUPINTMACRO(INTERRUPT7PORTANDPIN,INTERRUPT7PORT,digitalcallback7,INT7_vect)
#endif
// may need to add more in the future...

#ifdef PCINTERRUPT0PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint0=0;
   #ifndef PCINT0_7PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT0_7PORT PINB
      #elif ((PCINTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT0_7PORT PINC
      #elif ((PCINTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT0_7PORT PIND
      #elif ((PCINTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT0_7PORT PINE
      #elif ((PCINTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT0_7PORT PINF
      #elif ((PCINTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT0_7PORT PING
      #elif ((PCINTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT0_7PORT PINH
      #elif ((PCINTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT0_7PORT PINI
      #elif ((PCINTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT0_7PORT PINJ
      #elif ((PCINTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT0_7PORT PINK
      #elif ((PCINTERRUPT0PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT0_7PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT1PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint1=0;
   #ifndef PCINT0_7PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT0_7PORT PINB
      #elif ((PCINTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT0_7PORT PINC
      #elif ((PCINTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT0_7PORT PIND
      #elif ((PCINTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT0_7PORT PINE
      #elif ((PCINTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT0_7PORT PINF
      #elif ((PCINTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT0_7PORT PING
      #elif ((PCINTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT0_7PORT PINH
      #elif ((PCINTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT0_7PORT PINI
      #elif ((PCINTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT0_7PORT PINJ
      #elif ((PCINTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT0_7PORT PINK
      #elif ((PCINTERRUPT1PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT0_7PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT2PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint2=0;
   #ifndef PCINT0_7PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT0_7PORT PINB
      #elif ((PCINTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT0_7PORT PINC
      #elif ((PCINTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT0_7PORT PIND
      #elif ((PCINTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT0_7PORT PINE
      #elif ((PCINTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT0_7PORT PINF
      #elif ((PCINTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT0_7PORT PING
      #elif ((PCINTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT0_7PORT PINH
      #elif ((PCINTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT0_7PORT PINI
      #elif ((PCINTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT0_7PORT PINJ
      #elif ((PCINTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT0_7PORT PINK
      #elif ((PCINTERRUPT2PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT0_7PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT3PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint3=0;
   #ifndef PCINT0_7PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT0_7PORT PINB
      #elif ((PCINTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT0_7PORT PINC
      #elif ((PCINTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT0_7PORT PIND
      #elif ((PCINTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT0_7PORT PINE
      #elif ((PCINTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT0_7PORT PINF
      #elif ((PCINTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT0_7PORT PING
      #elif ((PCINTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT0_7PORT PINH
      #elif ((PCINTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT0_7PORT PINI
      #elif ((PCINTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT0_7PORT PINJ
      #elif ((PCINTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT0_7PORT PINK
      #elif ((PCINTERRUPT3PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT0_7PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT4PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint4=0;
   #ifndef PCINT0_7PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT0_7PORT PINB
      #elif ((PCINTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT0_7PORT PINC
      #elif ((PCINTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT0_7PORT PIND
      #elif ((PCINTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT0_7PORT PINE
      #elif ((PCINTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT0_7PORT PINF
      #elif ((PCINTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT0_7PORT PING
      #elif ((PCINTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT0_7PORT PINH
      #elif ((PCINTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT0_7PORT PINI
      #elif ((PCINTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT0_7PORT PINJ
      #elif ((PCINTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT0_7PORT PINK
      #elif ((PCINTERRUPT4PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT0_7PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT5PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint5=0;
   #ifndef PCINT0_7PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT0_7PORT PINB
      #elif ((PCINTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT0_7PORT PINC
      #elif ((PCINTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT0_7PORT PIND
      #elif ((PCINTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT0_7PORT PINE
      #elif ((PCINTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT0_7PORT PINF
      #elif ((PCINTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT0_7PORT PING
      #elif ((PCINTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT0_7PORT PINH
      #elif ((PCINTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT0_7PORT PINI
      #elif ((PCINTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT0_7PORT PINJ
      #elif ((PCINTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT0_7PORT PINK
      #elif ((PCINTERRUPT5PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT0_7PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT6PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint6=0;
   #ifndef PCINT0_7PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT0_7PORT PINB
      #elif ((PCINTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT0_7PORT PINC
      #elif ((PCINTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT0_7PORT PIND
      #elif ((PCINTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT0_7PORT PINE
      #elif ((PCINTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT0_7PORT PINF
      #elif ((PCINTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT0_7PORT PING
      #elif ((PCINTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT0_7PORT PINH
      #elif ((PCINTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT0_7PORT PINI
      #elif ((PCINTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT0_7PORT PINJ
      #elif ((PCINTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT0_7PORT PINK
      #elif ((PCINTERRUPT6PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT0_7PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT7PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint7=0;
   #ifndef PCINT0_7PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT0_7PORT PINB
      #elif ((PCINTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT0_7PORT PINC
      #elif ((PCINTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT0_7PORT PIND
      #elif ((PCINTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT0_7PORT PINE
      #elif ((PCINTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT0_7PORT PINF
      #elif ((PCINTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT0_7PORT PING
      #elif ((PCINTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT0_7PORT PINH
      #elif ((PCINTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT0_7PORT PINI
      #elif ((PCINTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT0_7PORT PINJ
      #elif ((PCINTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT0_7PORT PINK
      #elif ((PCINTERRUPT7PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT0_7PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT8PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint8=0;
   #ifndef PCINT8_15PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT8PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT8_15PORT PINB
      #elif ((PCINTERRUPT8PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT8_15PORT PINC
      #elif ((PCINTERRUPT8PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT8_15PORT PIND
      #elif ((PCINTERRUPT8PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT8_15PORT PINE
      #elif ((PCINTERRUPT8PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT8_15PORT PINF
      #elif ((PCINTERRUPT8PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT8_15PORT PING
      #elif ((PCINTERRUPT8PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT8_15PORT PINH
      #elif ((PCINTERRUPT8PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT8_15PORT PINI
      #elif ((PCINTERRUPT8PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT8_15PORT PINJ
      #elif ((PCINTERRUPT8PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT8_15PORT PINK
      #elif ((PCINTERRUPT8PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT8_15PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT9PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint9=0;
   #ifndef PCINT8_15PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT9PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT8_15PORT PINB
      #elif ((PCINTERRUPT9PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT8_15PORT PINC
      #elif ((PCINTERRUPT9PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT8_15PORT PIND
      #elif ((PCINTERRUPT9PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT8_15PORT PINE
      #elif ((PCINTERRUPT9PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT8_15PORT PINF
      #elif ((PCINTERRUPT9PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT8_15PORT PING
      #elif ((PCINTERRUPT9PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT8_15PORT PINH
      #elif ((PCINTERRUPT9PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT8_15PORT PINI
      #elif ((PCINTERRUPT9PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT8_15PORT PINJ
      #elif ((PCINTERRUPT9PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT8_15PORT PINK
      #elif ((PCINTERRUPT9PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT8_15PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT10PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint10=0;
   #ifndef PCINT8_15PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT10PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT8_15PORT PINB
      #elif ((PCINTERRUPT10PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT8_15PORT PINC
      #elif ((PCINTERRUPT10PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT8_15PORT PIND
      #elif ((PCINTERRUPT10PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT8_15PORT PINE
      #elif ((PCINTERRUPT10PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT8_15PORT PINF
      #elif ((PCINTERRUPT10PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT8_15PORT PING
      #elif ((PCINTERRUPT10PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT8_15PORT PINH
      #elif ((PCINTERRUPT10PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT8_15PORT PINI
      #elif ((PCINTERRUPT10PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT8_15PORT PINJ
      #elif ((PCINTERRUPT10PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT8_15PORT PINK
      #elif ((PCINTERRUPT10PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT8_15PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT11PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint11=0;
   #ifndef PCINT8_15PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT11PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT8_15PORT PINB
      #elif ((PCINTERRUPT11PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT8_15PORT PINC
      #elif ((PCINTERRUPT11PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT8_15PORT PIND
      #elif ((PCINTERRUPT11PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT8_15PORT PINE
      #elif ((PCINTERRUPT11PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT8_15PORT PINF
      #elif ((PCINTERRUPT11PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT8_15PORT PING
      #elif ((PCINTERRUPT11PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT8_15PORT PINH
      #elif ((PCINTERRUPT11PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT8_15PORT PINI
      #elif ((PCINTERRUPT11PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT8_15PORT PINJ
      #elif ((PCINTERRUPT11PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT8_15PORT PINK
      #elif ((PCINTERRUPT11PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT8_15PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT12PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint12=0;
   #ifndef PCINT8_15PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT12PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT8_15PORT PINB
      #elif ((PCINTERRUPT12PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT8_15PORT PINC
      #elif ((PCINTERRUPT12PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT8_15PORT PIND
      #elif ((PCINTERRUPT12PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT8_15PORT PINE
      #elif ((PCINTERRUPT12PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT8_15PORT PINF
      #elif ((PCINTERRUPT12PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT8_15PORT PING
      #elif ((PCINTERRUPT12PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT8_15PORT PINH
      #elif ((PCINTERRUPT12PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT8_15PORT PINI
      #elif ((PCINTERRUPT12PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT8_15PORT PINJ
      #elif ((PCINTERRUPT12PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT8_15PORT PINK
      #elif ((PCINTERRUPT12PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT8_15PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT13PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint13=0;
   #ifndef PCINT8_15PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT13PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT8_15PORT PINB
      #elif ((PCINTERRUPT13PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT8_15PORT PINC
      #elif ((PCINTERRUPT13PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT8_15PORT PIND
      #elif ((PCINTERRUPT13PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT8_15PORT PINE
      #elif ((PCINTERRUPT13PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT8_15PORT PINF
      #elif ((PCINTERRUPT13PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT8_15PORT PING
      #elif ((PCINTERRUPT13PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT8_15PORT PINH
      #elif ((PCINTERRUPT13PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT8_15PORT PINI
      #elif ((PCINTERRUPT13PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT8_15PORT PINJ
      #elif ((PCINTERRUPT13PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT8_15PORT PINK
      #elif ((PCINTERRUPT13PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT8_15PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT14PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint14=0;
   #ifndef PCINT8_15PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT14PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT8_15PORT PINB
      #elif ((PCINTERRUPT14PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT8_15PORT PINC
      #elif ((PCINTERRUPT14PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT8_15PORT PIND
      #elif ((PCINTERRUPT14PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT8_15PORT PINE
      #elif ((PCINTERRUPT14PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT8_15PORT PINF
      #elif ((PCINTERRUPT14PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT8_15PORT PING
      #elif ((PCINTERRUPT14PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT8_15PORT PINH
      #elif ((PCINTERRUPT14PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT8_15PORT PINI
      #elif ((PCINTERRUPT14PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT8_15PORT PINJ
      #elif ((PCINTERRUPT14PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT8_15PORT PINK
      #elif ((PCINTERRUPT14PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT8_15PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT15PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint15=0;
   #ifndef PCINT8_15PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT15PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT8_15PORT PINB
      #elif ((PCINTERRUPT15PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT8_15PORT PINC
      #elif ((PCINTERRUPT15PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT8_15PORT PIND
      #elif ((PCINTERRUPT15PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT8_15PORT PINE
      #elif ((PCINTERRUPT15PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT8_15PORT PINF
      #elif ((PCINTERRUPT15PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT8_15PORT PING
      #elif ((PCINTERRUPT15PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT8_15PORT PINH
      #elif ((PCINTERRUPT15PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT8_15PORT PINI
      #elif ((PCINTERRUPT15PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT8_15PORT PINJ
      #elif ((PCINTERRUPT15PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT8_15PORT PINK
      #elif ((PCINTERRUPT15PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT8_15PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT16PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint16=0;
   #ifndef PCINT16_23PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT16PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT16_23PORT PINB
      #elif ((PCINTERRUPT16PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT16_23PORT PINC
      #elif ((PCINTERRUPT16PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT16_23PORT PIND
      #elif ((PCINTERRUPT16PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT16_23PORT PINE
      #elif ((PCINTERRUPT16PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT16_23PORT PINF
      #elif ((PCINTERRUPT16PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT16_23PORT PING
      #elif ((PCINTERRUPT16PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT16_23PORT PINH
      #elif ((PCINTERRUPT16PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT16_23PORT PINI
      #elif ((PCINTERRUPT16PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT16_23PORT PINJ
      #elif ((PCINTERRUPT16PORTANDPIN & 0xF0)==DIGITALPORTK)      
         #define PCINT16_23PORT PINK
      #elif ((PCINTERRUPT16PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT16_23PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT17PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint17=0;
   #ifndef PCINT16_23PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT17PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT16_23PORT PINB
      #elif ((PCINTERRUPT17PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT16_23PORT PINC
      #elif ((PCINTERRUPT17PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT16_23PORT PIND
      #elif ((PCINTERRUPT17PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT16_23PORT PINE
      #elif ((PCINTERRUPT17PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT16_23PORT PINF
      #elif ((PCINTERRUPT17PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT16_23PORT PING
      #elif ((PCINTERRUPT17PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT16_23PORT PINH
      #elif ((PCINTERRUPT17PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT16_23PORT PINI
      #elif ((PCINTERRUPT17PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT16_23PORT PINJ
      #elif ((PCINTERRUPT17PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT16_23PORT PINK
      #elif ((PCINTERRUPT17PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT16_23PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT18PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint18=0;
   #ifndef PCINT16_23PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT18PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT16_23PORT PINB
      #elif ((PCINTERRUPT18PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT16_23PORT PINC
      #elif ((PCINTERRUPT18PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT16_23PORT PIND
      #elif ((PCINTERRUPT18PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT16_23PORT PINE
      #elif ((PCINTERRUPT18PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT16_23PORT PINF
      #elif ((PCINTERRUPT18PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT16_23PORT PING
      #elif ((PCINTERRUPT18PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT16_23PORT PINH
      #elif ((PCINTERRUPT18PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT16_23PORT PINI
      #elif ((PCINTERRUPT18PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT16_23PORT PINJ
      #elif ((PCINTERRUPT18PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT16_23PORT PINK
      #elif ((PCINTERRUPT18PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT16_23PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT19PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint19=0;
   #ifndef PCINT16_23PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT19PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT16_23PORT PINB
      #elif ((PCINTERRUPT19PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT16_23PORT PINC
      #elif ((PCINTERRUPT19PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT16_23PORT PIND
      #elif ((PCINTERRUPT19PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT16_23PORT PINE
      #elif ((PCINTERRUPT19PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT16_23PORT PINF
      #elif ((PCINTERRUPT19PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT16_23PORT PING
      #elif ((PCINTERRUPT19PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT16_23PORT PINH
      #elif ((PCINTERRUPT19PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT16_23PORT PINI
      #elif ((PCINTERRUPT19PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT16_23PORT PINJ
      #elif ((PCINTERRUPT19PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT16_23PORT PINK
      #elif ((PCINTERRUPT19PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT16_23PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT20PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint20=0;
   #ifndef PCINT16_23PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT20PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT16_23PORT PINB
      #elif ((PCINTERRUPT20PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT16_23PORT PINC
      #elif ((PCINTERRUPT20PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT16_23PORT PIND
      #elif ((PCINTERRUPT20PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT16_23PORT PINE
      #elif ((PCINTERRUPT20PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT16_23PORT PINF
      #elif ((PCINTERRUPT20PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT16_23PORT PING
      #elif ((PCINTERRUPT20PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT16_23PORT PINH
      #elif ((PCINTERRUPT20PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT16_23PORT PINI
      #elif ((PCINTERRUPT20PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT16_23PORT PINJ
      #elif ((PCINTERRUPT20PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT16_23PORT PINK
      #elif ((PCINTERRUPT20PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT16_23PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT21PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint21=0;
   #ifndef PCINT16_23PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT21PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT16_23PORT PINB
      #elif ((PCINTERRUPT21PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT16_23PORT PINC
      #elif ((PCINTERRUPT21PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT16_23PORT PIND
      #elif ((PCINTERRUPT21PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT16_23PORT PINE
      #elif ((PCINTERRUPT21PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT16_23PORT PINF
      #elif ((PCINTERRUPT21PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT16_23PORT PING
      #elif ((PCINTERRUPT21PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT16_23PORT PINH
      #elif ((PCINTERRUPT21PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT16_23PORT PINI
      #elif ((PCINTERRUPT21PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT16_23PORT PINJ
      #elif ((PCINTERRUPT21PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT16_23PORT PINK
      #elif ((PCINTERRUPT21PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT16_23PORT PINL
      #endif
   #endif
#endif


#ifdef PCINTERRUPT22PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint22=0;
   #ifndef PCINT16_23PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT22PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT16_23PORT PINB
      #elif ((PCINTERRUPT22PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT16_23PORT PINC
      #elif ((PCINTERRUPT22PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT16_23PORT PIND
      #elif ((PCINTERRUPT22PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT16_23PORT PINE
      #elif ((PCINTERRUPT22PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT16_23PORT PINF
      #elif ((PCINTERRUPT22PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT16_23PORT PING
      #elif ((PCINTERRUPT22PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT16_23PORT PINH
      #elif ((PCINTERRUPT22PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT16_23PORT PINI
      #elif ((PCINTERRUPT22PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT16_23PORT PINJ
      #elif ((PCINTERRUPT22PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT16_23PORT PINK
      #elif ((PCINTERRUPT22PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT16_23PORT PINL
      #endif
   #endif
#endif

#ifdef PCINTERRUPT23PORTANDPIN
   digitalcallbackfunctptr digitalcallbackpcint23=0;
   #ifndef PCINT16_23PORT
      // figure out the port for PCINT0_7
      #if ((PCINTERRUPT23PORTANDPIN & 0xF0)==DIGITALPORTB)
         #define PCINT16_23PORT PINB
      #elif ((PCINTERRUPT23PORTANDPIN & 0xF0)==DIGITALPORTC)
         #define PCINT16_23PORT PINC
      #elif ((PCINTERRUPT23PORTANDPIN & 0xF0)==DIGITALPORTD)
         #define PCINT16_23PORT PIND
      #elif ((PCINTERRUPT23PORTANDPIN & 0xF0)==DIGITALPORTE)
         #define PCINT16_23PORT PINE
      #elif ((PCINTERRUPT23PORTANDPIN & 0xF0)==DIGITALPORTF)
         #define PCINT16_23PORT PINF
      #elif ((PCINTERRUPT23PORTANDPIN & 0xF0)==DIGITALPORTG)
         #define PCINT16_23PORT PING
      #elif ((PCINTERRUPT23PORTANDPIN & 0xF0)==DIGITALPORTH)
         #define PCINT16_23PORT PINH
      #elif ((PCINTERRUPT23PORTANDPIN & 0xF0)==DIGITALPORTI)
         #define PCINT16_23PORT PINI
      #elif ((PCINTERRUPT23PORTANDPIN & 0xF0)==DIGITALPORTJ)
         #define PCINT16_23PORT PINJ
      #elif ((PCINTERRUPT23PORTANDPIN & 0xF0)==DIGITALPORTK)
         #define PCINT16_23PORT PINK
      #elif ((PCINTERRUPT23PORTANDPIN & 0xF0)==DIGITALPORTL)
         #define PCINT16_23PORT PINL
      #endif
   #endif
#endif



#define CHECKPCINTMACRO(PCINTERRUPTxPORTANDPIN,digitalcallbackpcintx)\
   {\
   unsigned char mask=(1<<(PCINTERRUPTxPORTANDPIN & 0x0F));\
   if ((changedpins & mask) && digitalcallbackpcintx!=0)\
      {\
      (digitalcallbackpcintx)(0,(newpins & mask)==0);\
      }\
   }


#ifdef PCINT0_7PORT
unsigned char interruptlastpinstate0_7=0;
ISR (PCINT0_vect)
   { // check for PCINT0 through PCINT7
   unsigned char newpins=PCINT0_7PORT;
   unsigned char changedpins=newpins^interruptlastpinstate0_7;
   interruptlastpinstate0_7=newpins;
   sei();
   #ifdef PCINTERRUPT0PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT0PORTANDPIN,digitalcallbackpcint0);
   #endif
   #ifdef PCINTERRUPT1PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT1PORTANDPIN,digitalcallbackpcint1);
   #endif
   #ifdef PCINTERRUPT2PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT2PORTANDPIN,digitalcallbackpcint2);
   #endif
   #ifdef PCINTERRUPT3PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT3PORTANDPIN,digitalcallbackpcint3);
   #endif
   #ifdef PCINTERRUPT4PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT4PORTANDPIN,digitalcallbackpcint4);
   #endif
   #ifdef PCINTERRUPT5PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT5PORTANDPIN,digitalcallbackpcint5);
   #endif
   #ifdef PCINTERRUPT6PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT6PORTANDPIN,digitalcallbackpcint6);
   #endif
   #ifdef PCINTERRUPT7PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT7PORTANDPIN,digitalcallbackpcint7);
   #endif
   }
#endif

#ifdef PCINT8_15PORT
unsigned char interruptlastpinstate8_15=0;
ISR (PCINT1_vect)
   { // check for PCINT8 through PCINT15
   unsigned char newpins=PCINT8_15PORT;
   unsigned char changedpins=newpins^interruptlastpinstate8_15;
   interruptlastpinstate8_15=newpins;
   sei();
   #ifdef PCINTERRUPT8PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT8PORTANDPIN,digitalcallbackpcint8);
   #endif
   #ifdef PCINTERRUPT9PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT9PORTANDPIN,digitalcallbackpcint9);
   #endif
   #ifdef PCINTERRUPT10PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT10PORTANDPIN,digitalcallbackpcint10);
   #endif
   #ifdef PCINTERRUPT11PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT11PORTANDPIN,digitalcallbackpcint11);
   #endif
   #ifdef PCINTERRUPT12PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT12PORTANDPIN,digitalcallbackpcint12);
   #endif
   #ifdef PCINTERRUPT13PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT13PORTANDPIN,digitalcallbackpcint13);
   #endif
   #ifdef PCINTERRUPT14PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT14PORTANDPIN,digitalcallbackpcint14);
   #endif
   #ifdef PCINTERRUPT15PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT15PORTANDPIN,digitalcallbackpcint15);
   #endif
   }
#endif
   
#ifdef PCINT16_23PORT
unsigned char interruptlastpinstate16_23=0;
ISR (PCINT2_vect)
   { // check for PCINT16 through PCINT23
   unsigned char newpins=PCINT16_23PORT;
   unsigned char changedpins=newpins^interruptlastpinstate16_23;
   interruptlastpinstate16_23=newpins;
   sei();
   #ifdef PCINTERRUPT16PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT16PORTANDPIN,digitalcallbackpcint16);
   #endif
   #ifdef PCINTERRUPT17PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT17PORTANDPIN,digitalcallbackpcint17);
   #endif
   #ifdef PCINTERRUPT18PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT18PORTANDPIN,digitalcallbackpcint18);
   #endif
   #ifdef PCINTERRUPT19PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT19PORTANDPIN,digitalcallbackpcint19);
   #endif
   #ifdef PCINTERRUPT20PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT20PORTANDPIN,digitalcallbackpcint20);
   #endif
   #ifdef PCINTERRUPT21PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT21PORTANDPIN,digitalcallbackpcint21);
   #endif
   #ifdef PCINTERRUPT22PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT22PORTANDPIN,digitalcallbackpcint22);
   #endif
   #ifdef PCINTERRUPT23PORTANDPIN
      CHECKPCINTMACRO(PCINTERRUPT23PORTANDPIN,digitalcallbackpcint23);
   #endif
   }
#endif



#define CHECKSETINTMACRO(INTERRUPTxPORTANDPIN,INTx,EICRx,ISCx0,digitalcallbackx)\
   if (pinnumber==INTERRUPTxPORTANDPIN)\
      {\
      digitalcallbackx=callback;\
      cli();\
      EICRx |= (1 << ISCx0);  /* enable rising and falling changes*/ \
      EIMSK |= (1 << INTx);  /* enable interrupt */\
      sei();\
      return;\
      }

#define CHECKSETPCINTMACRO(INTERRUPTxPORTANDPIN,digitalcallbackpcx,pcintnumberx)\
   if (pinnumber==INTERRUPTxPORTANDPIN)\
      {\
      digitalcallbackpcx=callback;\
      pcintnumber=pcintnumberx;\
      }


void lib_digitalio_setinterruptcallback(unsigned char pinnumber,digitalcallbackfunctptr callback)
   { // remember which input this interrupt belongs to and it's call back function   
#ifdef INTERRUPT0PORTANDPIN
   CHECKSETINTMACRO(INTERRUPT0PORTANDPIN,INT0,EICRA,ISC00,digitalcallback0);
#endif
#ifdef INTERRUPT1PORTANDPIN
   CHECKSETINTMACRO(INTERRUPT1PORTANDPIN,INT1,EICRA,ISC10,digitalcallback1);
#endif
#ifdef INTERRUPT2PORTANDPIN
   CHECKSETINTMACRO(INTERRUPT2PORTANDPIN,INT2,EICRA,ISC20,digitalcallback2);
#endif
#ifdef INTERRUPT3PORTANDPIN
   CHECKSETINTMACRO(INTERRUPT3PORTANDPIN,INT3,EICRA,ISC30,digitalcallback3);
#endif
#ifdef INTERRUPT4PORTANDPIN
   CHECKSETINTMACRO(INTERRUPT4PORTANDPIN,INT4,EICRB,ISC40,digitalcallback4);
#endif
#ifdef INTERRUPT5PORTANDPIN
   CHECKSETINTMACRO(INTERRUPT5PORTANDPIN,INT5,EICRB,ISC50,digitalcallback5);
#endif
#ifdef INTERRUPT6PORTANDPIN
   CHECKSETINTMACRO(INTERRUPT6PORTANDPIN,INT6,EICRB,ISC60,digitalcallback6);
#endif
#ifdef INTERRUPT7PORTANDPIN
   CHECKSETINTMACRO(INTERRUPT7PORTANDPIN,INT7,EICRB,ISC70,digitalcallback7);
#endif

unsigned char pcintnumber;

#ifdef PCINTERRUPT0PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT0PORTANDPIN,digitalcallbackpcint0,0);
#endif
#ifdef PCINTERRUPT1PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT1PORTANDPIN,digitalcallbackpcint1,1);
#endif
#ifdef PCINTERRUPT2PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT2PORTANDPIN,digitalcallbackpcint2,2);
#endif
#ifdef PCINTERRUPT3PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT3PORTANDPIN,digitalcallbackpcint3,3);
#endif
#ifdef PCINTERRUPT4PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT4PORTANDPIN,digitalcallbackpcint4,4);
#endif
#ifdef PCINTERRUPT5PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT5PORTANDPIN,digitalcallbackpcint5,5);
#endif
#ifdef PCINTERRUPT6PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT6PORTANDPIN,digitalcallbackpcint6,6);
#endif
#ifdef PCINTERRUPT7PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT7PORTANDPIN,digitalcallbackpcint7,7);
#endif
#ifdef PCINTERRUPT8PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT8PORTANDPIN,digitalcallbackpcint8,8);
#endif
#ifdef PCINTERRUPT9PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT9PORTANDPIN,digitalcallbackpcint9,9);
#endif
#ifdef PCINTERRUPT10PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT10PORTANDPIN,digitalcallbackpcint10,10);
#endif
#ifdef PCINTERRUPT11PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT11PORTANDPIN,digitalcallbackpcint11,11);
#endif
#ifdef PCINTERRUPT12PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT12PORTANDPIN,digitalcallbackpcint12,12);
#endif
#ifdef PCINTERRUPT13PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT13PORTANDPIN,digitalcallbackpcint13,13);
#endif
#ifdef PCINTERRUPT14PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT14PORTANDPIN,digitalcallbackpcint14,14);
#endif
#ifdef PCINTERRUPT15PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT15PORTANDPIN,digitalcallbackpcint15,15);
#endif
#ifdef PCINTERRUPT16PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT16PORTANDPIN,digitalcallbackpcint16,16);
#endif
#ifdef PCINTERRUPT17PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT17PORTANDPIN,digitalcallbackpcint17,17);
#endif
#ifdef PCINTERRUPT18PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT18PORTANDPIN,digitalcallbackpcint18,18);
#endif
#ifdef PCINTERRUPT19PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT19PORTANDPIN,digitalcallbackpcint19,19);
#endif
#ifdef PCINTERRUPT20PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT20PORTANDPIN,digitalcallbackpcint20,20);
#endif
#ifdef PCINTERRUPT21PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT21PORTANDPIN,digitalcallbackpcint21,21);
#endif
#ifdef PCINTERRUPT22PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT22PORTANDPIN,digitalcallbackpcint22,22);
#endif
#ifdef PCINTERRUPT23PORTANDPIN
   CHECKSETPCINTMACRO(PCINTERRUPT23PORTANDPIN,digitalcallbackpcint23,23);
#endif

   // enable the appropriate interrupts
#ifdef PCINT0_7PORT
   if (pcintnumber<=7)
      {
      interruptlastpinstate0_7=PCINT0_7PORT;
      PCICR |= (1<<PCIE0);
      PCMSK0 |= (1<<pcintnumber);
      }
#endif
#ifdef PCINT16_23PORT
   if (pcintnumber>=16)
      {
      interruptlastpinstate16_23=PCINT16_23PORT;
      PCICR |= (1<<PCIE2);
      PCMSK2 |= (1<<(pcintnumber-16));
      }
#endif
#ifdef PCINT8_15PORT
   if (pcintnunber>=8 && pcintnumber<=15)
      {
      interruptlastpinstate8_15=PCINT8_15PORT;
      PCICR |= (1<<PCIE1);
      PCMSK1 |= (1<<(pcintnumber-8));
      }
#endif
   sei();  // turn on interrupts in general
   }



