/*
 *  analogio.h
 *  Project
 *
 *  Created by Brad on 7/28/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

// ADC Functions:
//
// To use the ADC functions:
//
//    lib_analogio_setup(ADCREFVCC,0);         // initializes the ADC using Vcc as the reference
//                                        // special channel ADCCHANREF1POINT1 reads the 1.1V internal reference
//                                        // special channel ADCCHANSLEEP puts the adc to sleep to conserve power
//		lib_analogio_setchannel(0);							// sets which channel to read
//    lib_analogio_startreading();                  // starts a reading
//		while (!lib_analogio_readingdone()) {}			// wait for the reading to finish
//    unsigned int value=lib_analogio_getreading(); // gets the value when the reading is completed.
//
// 
//		or
//
//
//
//    lib_analogio_setup(ADCREFVCC,1);         // initializes the adc using Vcc as the reference with auto trigger
//                                        // special channel ADCCHANREF1POINT1 reads the 1.1V internal reference
//                                        // special channel ADCCHANSLEEP puts the adc to sleep to conserve power
//		lib_analogio_setchannel(0);							// sets which channel to read
//  
//		while ()
//			{
//			unsigned int value=lib_analogio_getreading(); // gets the value when the reading is completed.
//			}

// reference voltages:
#define ADCREFVCC 0
#define ADCREF1POINT1 1
#define ADCREFEXT 2

// channels: (channels 0 throuh ? can also be used)
#define ADCCHANREF1POINT1 -1
#define ADCCHANSLEEP -2

void lib_analogio_init(int reference,char autotrigger);
void lib_analogio_setchannel(int channel);
void lib_analogio_startreading();
unsigned int lib_analogio_getreading();
char lib_analogio_readingdone();
unsigned int lib_analogio_allinone(int channel,int reference);
