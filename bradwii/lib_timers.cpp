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
#include "lib_timers.h"

// This code creates a timer to count microseconds and includes support for using this timer to run multiple
// software timers.  The timer can run on either TIMER0 or TIMER1.  When run on TIMER1, a 16 bit timer is used
// which gives us microsecond resolution and less interrupt overhead.  When run on TIMER0, we get 4 microsecond
// resolution and more overhead.  Unsigned longs are used to store microseconds, so the longest intervals that
// can be measured without extra code is about 70 minutes.

// Usage:

// half second delay done multiple ways

// inittimers();
// unsigned long mytimer=lib_timers_starttimer();
// while (lib_timers_gettimermicroseconds(mytimer)<500000L) {}

// inittimers();
//    lib_timers_delaymilliseconds(500) {}


volatile unsigned long timeroverflowcount=0; // the number of overflows our counter has encountered

#ifdef USETIMER1FORGENERALTIMER

void lib_timers_init()
   { // needs to be called once in the program before timers can be used
   TCCR1B |= (1 << CS11); // Set up timer at Fcpu/8
   // at 16mhz, this gives 0.0000005 seconds per count
   // our 16 bit counter will overflow in 0.032768 seconds
   // We cascade this into an unsigned long.

   //Enable Overflow Interrupt Enable
   TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
   
   // make sure interrupts are enabled
   sei();
   }

ISR(TIMER1_OVF_vect)
   {
   // This is the interrupt service routine for TIMER1 OVERFLOW Interrupt.
   // Increment the overflow count
   ++timeroverflowcount;
   }
   
unsigned long lib_timers_getcurrentmicroseconds()
   { // returns microseconds since startup.  This mainly used internally because it wraps around.
   cli();
   unsigned int countervalue=TCNT1;
   long overflowcount=timeroverflowcount;
   sei();
   
   #if (F_CPU==16000000)
      return(((overflowcount<<16)+countervalue)>>1);
   #elif (F_CPU==8000000)
      return(((overflowcount<<16)+countervalue));
   #elif (F_CPU==1000000)
      return(((overflowcount<<16)+countervalue)<<3);
   #else
      need to add code for your clock rate
   #endif
   }

#else
void lib_timers_init()
   { // needs to be called once in the program before timers can be used
   TCCR0B |= (1 << CS01) | (1 << CS00); // Set up timer at Fcpu/64
   // at 16mhz, this gives 250,000 counts/second or 0.000004 seconds per count
   // our 8 bit counter will overflow in 0.001024 seconds

   //Enable Overflow Interrupt Enable
   TIMSK0 |= (1 << TOIE0);   // enable timer overflow interrupt
   
   // make sure interrupts are enabled
   sei();
   
   }

ISR(TIMER0_OVF_vect)
   {
   // This is the interrupt service routine for TIMER0 OVERFLOW Interrupt.
   // Increment the overflow count 
   ++timeroverflowcount;
   }
   
unsigned long lib_timers_getcurrentmicroseconds()
   { // returns microseconds since startup.  This mainly used internally because it wraps around.
   cli();
   unsigned char countervalue=TCNT0;
   long overflowcount=timeroverflowcount;
   sei();
   
   #if (F_CPU==16000000)
      return(((overflowcount<<8)+countervalue)<<2);
   #elif (F_CPU==8000000)
      return(((overflowcount<<8)+countervalue)<<3);
   #elif (F_CPU==1000000)
      return(((overflowcount<<8)+countervalue)<<6);
   #else
      need to add code for your clock rate
   #endif
   }
#endif

unsigned long lib_timers_gettimermicroseconds(unsigned long starttime)
   { // returns microseconds since this timer was started
   unsigned long currenttime=lib_timers_getcurrentmicroseconds();
   if (starttime>currenttime) // we have wrapped around
      {
      return(0xFFFFFFFF-starttime+currenttime);
      }
   else
      {
      return(currenttime-starttime);
      }
   }
   
unsigned long lib_timers_gettimermicrosecondsandreset(unsigned long *starttime)
   { // returns microseconds since this timer was started and then reset the start time
   // this allows us to keep checking the time without losing any time
   unsigned long currenttime=lib_timers_getcurrentmicroseconds();
   unsigned long returnvalue;
   
   if (*starttime>currenttime) // we have wrapped around
      {
      returnvalue=(0xFFFFFFFF-*starttime+currenttime);
      }
   else
      {
      returnvalue=(currenttime-*starttime);
      }
   *starttime=currenttime;
   return(returnvalue);
   }

unsigned long lib_timers_starttimer()
   { // start a timer
   return(lib_timers_getcurrentmicroseconds());
   }
   
void    lib_timers_delaymilliseconds(unsigned long delaymilliseconds)
   {
   unsigned long timercounts=lib_timers_starttimer();
   while (lib_timers_gettimermicroseconds(timercounts)<delaymilliseconds*1000L) {}
   }
