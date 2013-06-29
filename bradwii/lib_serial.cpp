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

// This library manages serial ports.  It sets up input and output buffers and sends and receieves using interrupts.
// To use this code, define which ports to include and the size of their buffers in projectsettings.h
// Then use the library like this:
//
// lib_serial_initport(2,9600); // init serial port 2 to 9600 baud
// lib_serial_sendstring(2,"Send this string");
// lib_serial_sendchar(2,'.');
// lib_serial_senddata(2,"This is Data",4); // sends the first 4 characters of the string
// int count=lib_serial_numcharsavailable(2);
// if (count>0)
//      {
//      lib_serial_getdata(2,data,count);
//      }

#include <avr/io.h>
#include <avr/interrupt.h>
#include "lib_serial.h"
#include "projectsettings.h"

// set the USESERIALPORTx flags and buffer sizes in projectsettings.h
// also set SERIALxOUTPUTBUFFERSIZE and SERIALxINPUTBUFFERSIZE projectsettings.h
#ifdef USESERIALPORT0
unsigned char serialtxbuffer0[SERIAL0OUTPUTBUFFERSIZE];
unsigned char serialrxbuffer0[SERIAL0INPUTBUFFERSIZE];
#endif
#ifdef USESERIALPORT1
unsigned char serialtxbuffer1[SERIAL1OUTPUTBUFFERSIZE];
unsigned char serialrxbuffer1[SERIAL1INPUTBUFFERSIZE];
#endif
#ifdef USESERIALPORT2
unsigned char serialtxbuffer2[SERIAL2OUTPUTBUFFERSIZE];
unsigned char serialrxbuffer2[SERIAL2INPUTBUFFERSIZE];
#endif
#ifdef USESERIALPORT3
unsigned char serialtxbuffer3[SERIAL3OUTPUTBUFFERSIZE];
unsigned char serialrxbuffer3[SERIAL3INPUTBUFFERSIZE];
#endif

#ifdef USESERIALPORTUSB
  #include "lib_usb.h"
#endif


#ifdef USESERIALPORT3
   #define NUMSERIALPORTS 4
#else
   #ifdef USESERIALPORT2
      #define NUMSERIALPORTS 3
   #else
      #ifdef USESERIALPORT1
         #define NUMSERIALPORTS 2
      #else
         #define NUMSERIALPORTS 1
      #endif
   #endif
#endif

// for processors that don't number their serial ports:
// this hasn't been tested
#ifndef UCSR0B
#define UCSR0B UCSRB
#define RXEN0 RXEN
#define TXEN0 TXEN
#define UCSZ00 UCSZ0
#define UCSZ01 UCSZ1
#define UBRR0L UBRRL
#define UBRR0H UBRRH
#define RXCIE0 RXCIE
#define UDR0 UDR
#endif

unsigned char *serialtxbuffer[NUMSERIALPORTS];
int serialtxbuffersize[NUMSERIALPORTS];
volatile int serialtxbufferstartindex[NUMSERIALPORTS];
volatile int serialtxbufferendindex[NUMSERIALPORTS];
unsigned char *serialrxbuffer[NUMSERIALPORTS];
int serialrxbuffersize[NUMSERIALPORTS];
int serialrxbufferstartindex[NUMSERIALPORTS];
volatile int serialrxbufferendindex[NUMSERIALPORTS];
serialcallbackfunctptr serialrxcallback[NUMSERIALPORTS];

int lib_serial_availableoutputbuffersize(unsigned char serialportnumber)
   { // returns how many more bytes can fit in the outputbuffer
   if (serialtxbufferstartindex[serialportnumber]>serialtxbufferendindex[serialportnumber])
      return(serialtxbufferstartindex[serialportnumber]-serialtxbufferendindex[serialportnumber]-1);
   else
      return(serialtxbuffersize[serialportnumber]-(serialtxbufferendindex[serialportnumber]-serialtxbufferstartindex[serialportnumber])-1);
   }

void lib_serial_setrxcallback(unsigned char serialportnumber,serialcallbackfunctptr callback)
   {
   serialrxcallback[serialportnumber]=callback;
   }

#define INITSERIALPORTMACRO(portnumber,ucsra,ucsrb,uscrc,rxen,txen,uscz0,uscz1,ubrrl,ubrrh,rxcie,utx,txbuffer,rxbuffer,txsize,rxsize)\
   if (serialportnumber==portnumber)\
      {\
      serialtxbuffer[portnumber]=txbuffer;\
      serialtxbuffersize[portnumber]=txsize;\
      serialrxbuffer[portnumber]=rxbuffer;\
      serialrxbuffersize[portnumber]=rxsize;\
      \
      ucsra  = (1<<utx);\
      ucsrb |= (1 << rxen) | (1 << txen);   /* Turn on the transmission and reception circuitry */\
      uscrc |=  (1 << uscz0) | (1 << uscz1); /* Use 8-bit character sizes */\
      \
      ubrrl = baudprescale; /* Load lower 8-bits of the baud rate value into the low byte of the UBRR register */\
      ubrrh = (baudprescale >> 8); /* Load upper 8-bits of the baud rate value into the high byte of the UBRR register */\ 
      \
      ucsrb |= (1 << rxcie); /* Enable the USART Recieve Complete */\
      }

void lib_serial_initport(unsigned char serialportnumber,long baud)
   { // initialize the serial port and set up a read buffer and interrupts so that we don't lose any data from reading too slowly
#ifdef USESERIALPORTUSB
   if (serialportnumber==USBPORTNUMBER)
      {
      lib_usb_init();   
      return;
      }
#endif

  unsigned long baudprescale=((F_CPU  / 4 / baud -1) / 2);
   serialtxbufferstartindex[serialportnumber]=serialtxbufferendindex[serialportnumber]=0;
   serialrxbufferstartindex[serialportnumber]=serialrxbufferendindex[serialportnumber]=0;

#ifdef USESERIALPORT0
   INITSERIALPORTMACRO(0,UCSR0A,UCSR0B,UCSR0C,RXEN0,TXEN0,UCSZ00,UCSZ01,UBRR0L,UBRR0H,RXCIE0,U2X0,serialtxbuffer0,serialrxbuffer0,SERIAL0OUTPUTBUFFERSIZE,SERIAL0INPUTBUFFERSIZE);
#endif
#ifdef USESERIALPORT1
   INITSERIALPORTMACRO(1,UCSR1A,UCSR1B,UCSR1C,RXEN1,TXEN1,UCSZ10,UCSZ11,UBRR1L,UBRR1H,RXCIE1,U2X1,serialtxbuffer1,serialrxbuffer1,SERIAL1OUTPUTBUFFERSIZE,SERIAL1INPUTBUFFERSIZE);
#endif
#ifdef USESERIALPORT2
   INITSERIALPORTMACRO(2,UCSR2A,UCSR2B,UCSR2C,RXEN2,TXEN2,UCSZ20,UCSZ21,UBRR2L,UBRR2H,RXCIE2,U2X2,serialtxbuffer2,serialrxbuffer2,SERIAL2OUTPUTBUFFERSIZE,SERIAL2INPUTBUFFERSIZE);
#endif
#ifdef USESERIALPORT3
   INITSERIALPORTMACRO(3,UCSR3A,UCSR3B,UCSR3C,RXEN3,TXEN3,UCSZ30,UCSZ31,UBRR3L,UBRR3H,RXCIE3,U2X3,serialtxbuffer3,serialrxbuffer3,SERIAL3OUTPUTBUFFERSIZE,SERIAL3INPUTBUFFERSIZE);
#endif
   sei(); // turn on all interrupts
   }

#define SERIALSENDTHENEXTCHARMACRO(portnum,ucsrb,udrie,udr)\
   if (serialportnumber==portnum)\
      {\
      if (serialtxbufferstartindex[serialportnumber]==serialtxbufferendindex[serialportnumber]) \
         { /* there's nothing left to send */\
         /* turn off the transmit registre empty interrupt so it doesn't keep firing.*/\
         /* we also use this as a flag to tell us whether we are currently sending or not.*/\
         ucsrb &= ~(1 << udrie);\
         }\
      else\
         {\
         udr = serialtxbuffer[serialportnumber][serialtxbufferstartindex[serialportnumber]++];\
         ucsrb |= ( 1 << udrie ); /* turn on the interrupt to tell us when we are ready to send another character*/\
         if (serialtxbufferstartindex[serialportnumber]==serialtxbuffersize[serialportnumber]) serialtxbufferstartindex[serialportnumber]=0;\
         }\
      }
         
         
void serialsendthenextchar(unsigned char serialportnumber)
   { // this should only be called internally.  It sends the next character in the send buffer to the hardware
#ifdef USESERIALPORT0
   SERIALSENDTHENEXTCHARMACRO(0,UCSR0B,UDRIE0,UDR0);
#endif
#ifdef USESERIALPORT1
   SERIALSENDTHENEXTCHARMACRO(1,UCSR1B,UDRIE1,UDR1);
#endif
#ifdef USESERIALPORT2
   SERIALSENDTHENEXTCHARMACRO(2,UCSR2B,UDRIE2,UDR2);
#endif
#ifdef USESERIALPORT3
   SERIALSENDTHENEXTCHARMACRO(3,UCSR3B,UDRIE3,UDR3);
#endif
   }

#define SERIALSENDCHARMACRO(portnumber,ucsrb,udrie)\
   if (serialportnumber==portnumber && !(ucsrb & ( 1 << udrie ))) serialsendthenextchar(serialportnumber);

void lib_serial_sendchar(unsigned char serialportnumber,unsigned char c)
   { // add a character to the send buffer
#ifdef USESERIALPORTUSB
   if (serialportnumber==USBPORTNUMBER)
      {
      lib_usb_send(USB_CDC_TX,&c,1);
      return;
      }
#endif
      
   sei();
//   cli(); // disable interrupts while we do this so the interrupt handler doesn't change things on us
   serialtxbuffer[serialportnumber][serialtxbufferendindex[serialportnumber]++]=c;
   if (serialtxbufferendindex[serialportnumber]==serialtxbuffersize[serialportnumber]) serialtxbufferendindex[serialportnumber]=0;

   // if we aren't already sending something, we need to get the ball rolling by sending the first character
   // if the transmit buffer empty interrupt flag isn't set then we aren't currently sending anything.
#ifdef USESERIALPORT0
   SERIALSENDCHARMACRO(0,UCSR0B,UDRIE0);
#endif
#ifdef USESERIALPORT1
   SERIALSENDCHARMACRO(1,UCSR1B,UDRIE1);
#endif
#ifdef USESERIALPORT2
   SERIALSENDCHARMACRO(2,UCSR2B,UDRIE2);
#endif
#ifdef USESERIALPORT3
   SERIALSENDCHARMACRO(3,UCSR3B,UDRIE3);
#endif
//   sei();
   }
   
void lib_serial_sendstring(unsigned char serialportnumber,char *string)
   { // adds the string to the output buffer.
   while (*string) lib_serial_sendchar(serialportnumber,*string++);
   }

void lib_serial_senddata(unsigned char serialportnumber,unsigned char *data,int datalength)
   { // send datalength bytes of data to the serial port
   while (datalength-- >0) lib_serial_sendchar(serialportnumber,*data++);
   }

int lib_serial_numcharsavailable(unsigned char serialportnumber)
   { // returns number of characters available in the rx buffer
   int returnvalue;
#ifdef USESERIALPORTUSB
   if (serialportnumber==USBPORTNUMBER)
      {
      return(lib_usb_charsavailable(USB_CDC_RX));
      }
#endif

   int endindex=serialrxbufferendindex[serialportnumber];
   
   returnvalue=endindex-serialrxbufferstartindex[serialportnumber];
   if (serialrxbufferstartindex[serialportnumber]>endindex)
      returnvalue+=serialrxbuffersize[serialportnumber];
   return(returnvalue);
   }

unsigned char lib_serial_getchar(unsigned char serialportnumber)
   { // get the next character from the serial port
   // I don't think we need to disable interrupts here since the interrupt handler shouldn't be
   // changing any of the things we are looking at here.
#ifdef USESERIALPORTUSB
   if (serialportnumber==USBPORTNUMBER)
      {
      return(lib_usb_recv(USB_CDC_RX));
      }
#endif

   unsigned char returnvalue=serialrxbuffer[serialportnumber][serialrxbufferstartindex[serialportnumber]++];
   if (serialrxbufferstartindex[serialportnumber]==serialrxbuffersize[serialportnumber]) serialrxbufferstartindex[serialportnumber]=0;
   return(returnvalue);
   }

void lib_serial_getdata(unsigned char serialportnumber,unsigned char *data,int numchars)
   {
   for (int x=0;x<numchars;++x)
      *data++=lib_serial_getchar(serialportnumber);
   }

void serialaddcharactertoreceivebuffer(unsigned char serialportnumber,unsigned char c)
   { // used internally by the interrupt handler
   sei();
   
   // if there's a callback associated with this port, use it instead of our buffer
   if (serialrxcallback[serialportnumber])
      {
      (*serialrxcallback[serialportnumber])(c);
      }
   else
      {
      serialrxbuffer[serialportnumber][serialrxbufferendindex[serialportnumber]++]=c;
      if (serialrxbufferendindex[serialportnumber]==serialrxbuffersize[serialportnumber]) serialrxbufferendindex[serialportnumber]=0;
      }
   }
   
#define SERIALISRMACRO(number,txvector,rxvector,udr)\
ISR(txvector) \
   { /* we just finished sending a character from our transmit buffer.  Send the next one.*/\
   serialsendthenextchar(number);\
   }\
\
ISR(rxvector)\
   { /* we just received an incoming character*/\
   unsigned char c=udr;\
   serialaddcharactertoreceivebuffer(number,c);\
   }

// This interrupt gets called when the transmit buffer is ready for another byte.
#ifdef USESERIALPORT0

// unfortunately, different processors use different names for their interrupt vectors
#ifdef USART_RX_vect
   #define USART0_RX_vect USART_RX_vect
#else
   #ifdef USART_RXC_vect
      #define USART0_RX_vect USART_RXC_vect
   #endif
#endif

#ifdef USART_UDRE_vect
   #define USART0_UDRE_vect USART_UDRE_vect
#endif


SERIALISRMACRO(0,USART0_UDRE_vect,USART0_RX_vect,UDR0);
#endif

#ifdef USESERIALPORT1
SERIALISRMACRO(1,USART1_UDRE_vect,USART1_RX_vect,UDR1);
#endif

#ifdef USESERIALPORT2
SERIALISRMACRO(2,USART2_UDRE_vect,USART2_RX_vect,UDR2);
#endif

#ifdef USESERIALPORT3
SERIALISRMACRO(3,USART3_UDRE_vect,USART3_RX_vect,UDR3);
#endif

