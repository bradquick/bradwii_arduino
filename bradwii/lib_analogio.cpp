/*
 *  analogio.c
 *  Project
 *
 *  Created by Brad on 2/2/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include <avr/io.h>
#include "lib_analogio.h"

void lib_analogio_init(int reference,char autotrigger)
	{ // starts the analog to digital converter and sets which reference voltage to use
   if (reference==ADCCHANSLEEP)
      {
      ADCSRA &= ~(1 << ADEN);  // disable ADC to save power
      }
   else
      {
	  
      if (reference==ADCREFVCC)
         ADMUX=(1 << REFS0); // Set the reference voltage to Vcc
      else if (reference==ADCREF1POINT1)
         ADMUX=(1 << REFS0) | (1 << REFS1); // Set the reference voltage to 1.1v internal reference
	   else if (reference==ADCREFEXT)
		   ADMUX=0;
			
		if (autotrigger) ADCSRA |= (1 << ADATE);  // set auto trigger (free run mode)

      ADCSRA = (1 << ADPS2) | (1 << ADPS1); // Set ADC prescalar to 64 - 125KHz sample rate @ 8MHz. Should end up between 50KHZ and 200 KHZ CHANGE WITH F_CPU

      ADCSRA |= (1 << ADEN);  // Enable ADC 
		}
		
	// default to channel zero
	lib_analogio_setchannel(0);
	}
	
void lib_analogio_setchannel(int channel)
   { // set up and enable the adc to read channel channel
	// we can read the reference voltage instead of a channel if we want, otherwise channels go from
	// 0-7
	ADMUX &= (~0x07); // clear out the old channel
	
	if (channel==ADCCHANREF1POINT1)
//         ADMUX |= 0x0E; // Set ADC reference to AVCC and mux channel to 1.1v internal
		ADMUX |= (1<<MUX3) | (1<<MUX2) | (1<<MUX1); // Set ADC reference to AVCC and mux channel to 1.1v internal
	else
		ADMUX |= channel; // Set ADC reference to AVCC and mux channel to channel	  
   }

void lib_analogio_startreading()
	{ // don't need to do if auto triggering
   ADCSRA |= (1 << ADIF);  // reset the completion flag.  
   ADCSRA |= (1 << ADSC);  // Start a conversion 	  
	}
	
unsigned int lib_analogio_getreading()
   {
   // get the reading
   unsigned int result=ADCL;
   result|=ADCH<<8;
   
   return(result);
   }

char lib_analogio_readingdone()
	{ // returns non-zero if the reading is completed
   return (ADCSRA & (1 << ADIF));  // return non zero if conversion is complete 
	}
	
unsigned int lib_analogio_allinone(int channel,int reference)
	{ // reads the adc in one call.  It's slow since we wait for the conversion
	lib_analogio_init(reference,0);
	lib_analogio_setchannel(channel);
	lib_analogio_startreading();
	while (!lib_analogio_readingdone()) {}
	return(lib_analogio_getreading());
	}
