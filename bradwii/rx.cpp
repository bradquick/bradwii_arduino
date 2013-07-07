/* 
Copyright 2013 Brad Quick

Some of this code is based on Multiwii code by Alexandre Dubus (www.multiwii.com)

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

 
#include "bradwii.h"
#include "rx.h"
#include "defs.h"
#include "lib_timers.h"
#include "lib_digitalio.h"
#include "lib_serial.h"
#include <avr/io.h>

// when adding new receivers, the following functions must be included:
// initrx()   // initializes the r/c receiver
// readrx()   // loads global.rxvalues with r/c values as fixedpointnum's from -1 to 1 (0 is the center).

extern globalstruct global;

#if (RX_TYPE==RX_NORMAL)

unsigned long rxtimer[RXNUMCHANNELS];
volatile unsigned int rxrawvalues[RXNUMCHANNELS];

#define processcallback(INDEX)                                             \
   {                                                                              \
   if (newstate)                                                                  \
      {                                                                           \
      unsigned int value=lib_timers_gettimermicroseconds(rxtimer[INDEX]);                  \
      if (value>900 && value<2050) rxrawvalues[INDEX]=value;            \
      }                                                                           \
   else rxtimer[INDEX]=lib_timers_starttimer();                                             \
   }

void throttlecallback(unsigned char interruptnumber,unsigned char newstate) // call back will get called any time the pin changes
   {
   processcallback(THROTTLEINDEX);
   
   // reset the failsafe timer
   global.failsafetimer=lib_timers_starttimer();
   }

void rollcallback(unsigned char interruptnumber,unsigned char newstate) // call back will get called any time the pin changes
   {
   processcallback(ROLLINDEX);
   }

void pitchcallback(unsigned char interruptnumber,unsigned char newstate) // call back will get called any time the pin changes
   {
   processcallback(PITCHINDEX);
   }

void yawcallback(unsigned char interruptnumber,unsigned char newstate) // call back will get called any time the pin changes
   {
   processcallback(YAWINDEX);
   }

void aux1callback(unsigned char interruptnumber,unsigned char newstate) // call back will get called any time the pin changes
   {
   processcallback(AUX1INDEX);
   }

void aux2callback(unsigned char interruptnumber,unsigned char newstate) // call back will get called any time the pin changes
   {
   processcallback(AUX2INDEX);
   }

#if (RXNUMCHANNELS>6)
void aux3callback(unsigned char interruptnumber,unsigned char newstate) // call back will get called any time the pin changes
   {
   processcallback(AUX3INDEX);
   }

void aux4callback(unsigned char interruptnumber,unsigned char newstate) // call back will get called any time the pin changes
   {
   processcallback(AUX4INDEX);
   }
#endif

void initrx()
   {
   for (int x=0;x<RXNUMCHANNELS;++x)
      global.rxvalues[x]=0; // middle position
   global.rxvalues[THROTTLEINDEX]=FPTHROTTLELOW; // default throttle to low position
      
   lib_digitalio_initpin(THROTTLE_RX_INPUT,DIGITALINPUT);
   lib_digitalio_setinterruptcallback(THROTTLE_RX_INPUT,throttlecallback);    

   lib_digitalio_initpin(ROLL_RX_INPUT,DIGITALINPUT);
   lib_digitalio_setinterruptcallback(ROLL_RX_INPUT,rollcallback); 

   lib_digitalio_initpin(PITCH_RX_INPUT,DIGITALINPUT);
   lib_digitalio_setinterruptcallback(PITCH_RX_INPUT,pitchcallback); 

   lib_digitalio_initpin(YAW_RX_INPUT,DIGITALINPUT);
   lib_digitalio_setinterruptcallback(YAW_RX_INPUT,yawcallback); 

#if (RXNUMCHANNELS>4)
   lib_digitalio_initpin(AUX1_RX_INPUT,DIGITALINPUT);
   lib_digitalio_setinterruptcallback(AUX1_RX_INPUT,aux1callback); 
#endif

#if (RXNUMCHANNELS>5)
   lib_digitalio_initpin(AUX2_RX_INPUT,DIGITALINPUT);
   lib_digitalio_setinterruptcallback(AUX2_RX_INPUT,aux2callback); 
#endif

#if (RXNUMCHANNELS>6)
   lib_digitalio_initpin(AUX3_RX_INPUT,DIGITALINPUT);
   lib_digitalio_setinterruptcallback(AUX3_RX_INPUT,aux3callback); 
#endif

#if (RXNUMCHANNELS>7)
   lib_digitalio_initpin(AUX4_RX_INPUT,DIGITALINPUT);
   lib_digitalio_setinterruptcallback(AUX4_RX_INPUT,aux4callback); 
#endif
   }

void readrx()
   {
   for (int x=0;x<RXNUMCHANNELS;++x)
      {
      // convert from 1000-2000 range to -1 to 1 fixedpointnum range and low pass filter to remove glitches
      lib_fp_lowpassfilter(&global.rxvalues[x], ((fixedpointnum)rxrawvalues[x]-1500)*131L, global.timesliver, FIXEDPOINTONEOVERONESIXTYITH, TIMESLIVEREXTRASHIFT);
      }
   }

#elif (RX_TYPE==RX_DSM2_1024 || RX_TYPE==RX_DSM2_2048)

#if (RX_TYPE==RX_DSM2_1024)
   #define DSM2_CHAN_SHIFT  2       // Assumes 10 bit frames, that is 1024 mode.
   #define DSM2_CHAN_MASK   0x03    // Assumes 10 bit frames, that is 1024 mode.
#else
   #define DSM2_CHAN_SHIFT  3       // Assumes 11 bit frames, that is 2048 mode.
   #define DSM2_CHAN_MASK   0x07    // Assumes 11 bit frames, that is 2048 mode.
#endif

#define DSM2_BAUD 115200L

#define DSM2STATE_NEWFRAMESARTED 0
#define DSM2STATE_WAITINGFORFIRSTCHAR 1
#define DSM2STATE_WAITINGFORSECONDCHAR 2

#ifndef RX_CHANNEL_ORDER
   #define RX_CHANNEL_ORDER         THROTTLEINDEX,ROLLINDEX,PITCHINDEX,YAWINDEX,AUX1INDEX,AUX2INDEX,AUX3INDEX,AUX4INDEX,8,9,10,11 //For Graupner/Spektrum
#endif

volatile unsigned int rxrawvalues[RXNUMCHANNELS];
unsigned long dsm2timer;
unsigned char dsm2state;
unsigned char dsm2firstchar;
unsigned char dsm2channelindex[]={RX_CHANNEL_ORDER};

// this callback will get called whenever we receive a character on our dsm2 serial port
void dsm2serialcallback(unsigned char c)
   {
   // there is a 11ms delay between packets.  We need to use this delay to determine when we are
   // getting new data.
   unsigned long microsecondssincelastchar=lib_timers_gettimermicrosecondsandreset(&dsm2timer);
   if (microsecondssincelastchar>5000)
      { // this is a new packet.  Skip this first byte
      dsm2state=DSM2STATE_NEWFRAMESARTED;
      }
   else if (dsm2state==DSM2STATE_NEWFRAMESARTED)
      { // skip the 2nd byte too
      dsm2state=DSM2STATE_WAITINGFORFIRSTCHAR;
      }
   else if (dsm2state==DSM2STATE_WAITINGFORFIRSTCHAR)
      {
      dsm2firstchar=c;
      dsm2state=DSM2STATE_WAITINGFORSECONDCHAR;
      }
   else
      {
      unsigned char channel = 0x0F & (dsm2firstchar >> DSM2_CHAN_SHIFT);
      
      if (channel < RXNUMCHANNELS)
         {
         channel=dsm2channelindex[channel];
         rxrawvalues[channel] = ((unsigned int)(dsm2firstchar & DSM2_CHAN_MASK) << 8) + c;
         }
      dsm2state=DSM2STATE_WAITINGFORFIRSTCHAR;

      // reset the failsafe timer
      global.failsafetimer=lib_timers_starttimer();
      }
   }
   
void initrx()
   {
   dsm2timer=lib_timers_starttimer();
   
   lib_serial_initport(RX_DSM2_SERIAL_PORT,DSM2_BAUD);
   lib_serial_setrxcallback(RX_DSM2_SERIAL_PORT,dsm2serialcallback);
   }   

void readrx()
   {
   for (int x=0;x<RXNUMCHANNELS;++x)
      {
#if (RX_TYPE==RX_DSM2_1024)
      // convert from 0-1024 range to -1 to 1 fixedpointnum range and low pass filter to remove glitches
      lib_fp_lowpassfilter(&global.rxvalues[x], ((fixedpointnum)rxrawvalues[x]-512)<<7, global.timesliver, FIXEDPOINTONEOVERONESIXTYITH, TIMESLIVEREXTRASHIFT);
#else
      // convert from 0-2048 range to -1 to 1 fixedpointnum range and low pass filter to remove glitches
      lib_fp_lowpassfilter(&global.rxvalues[x], ((fixedpointnum)rxrawvalues[x]-1024)<<6, global.timesliver, FIXEDPOINTONEOVERONESIXTYITH, TIMESLIVEREXTRASHIFT);
#endif
      }
   }


#elif (RX_TYPE==RX_CPPM)

#ifndef RX_CHANNEL_ORDER
   #define RX_CHANNEL_ORDER         THROTTLEINDEX,ROLLINDEX,PITCHINDEX,YAWINDEX,AUX1INDEX,AUX2INDEX,AUX3INDEX,AUX4INDEX,8,9,10,11 //For Graupner/Spektrum
#endif

unsigned char channelindex[]={RX_CHANNEL_ORDER};
volatile unsigned int rxrawvalues[RXNUMCHANNELS];

void serialsumcallback(unsigned char interruptnumber, unsigned char newstate) 
   { // gke
   static unsigned long starttime = 0;
   static unsigned char chan = 0;
   unsigned int width;
   static unsigned int glitches=0;
  
   if (newstate)
      {
      width = lib_timers_gettimermicrosecondsandreset(&starttime);
      
      if ( width > 3000)
         {
         global.failsafetimer=lib_timers_starttimer();  // reset the failsafe timer
         chan = 0;  
         }
      else if (chan < RXNUMCHANNELS)
         {
         if (width>900 && width<2050)
            rxrawvalues[chan] = width;
//         else
//            {
//            global.debugvalue[1] = width;
//            global.debugvalue[0] = ++glitches;
//            }
          
         chan++;
         }
      } 
   }

void readrx()
   {
   unsigned char chan;
   
   for (chan=0; chan<RXNUMCHANNELS;++chan) // convert from 1000-2000 range to -1 to 1 fixedpointnum range and low pass filter to remove glitches
      lib_fp_lowpassfilter(&global.rxvalues[channelindex[chan]], ((fixedpointnum)rxrawvalues[chan]-1500)*131L, global.timesliver, FIXEDPOINTONEOVERONESIXTYITH, TIMESLIVEREXTRASHIFT);
   }

void initrx()
   {
   for (int x=0;x<RXNUMCHANNELS;++x)
      global.rxvalues[x]=0; // middle position
   global.rxvalues[THROTTLEINDEX]=FPTHROTTLELOW; // default throttle to low position
      
   lib_digitalio_initpin(THROTTLE_RX_INPUT,DIGITALINPUT);
   lib_digitalio_setinterruptcallback(THROTTLE_RX_INPUT, serialsumcallback);   
   }

#endif