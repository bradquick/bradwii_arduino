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

// Define the pwm outputs that you want to use in projectsettings.h

#include <avr/io.h>
#include "lib_digitalio.h"
#include "lib_pwm.h"

#ifdef USEPWM0
void lib_pwm_init0(unsigned char mode,unsigned char output,unsigned char prescaler)
   {
   OCR0A=0;
   OCR0B=0;
   TCCR0A |=mode | output; // mode for switching on and off
   
   TCCR0B |=prescaler; 
   }

void lib_pwm_setduty0A(unsigned char value)
   {
   OCR0A=value;  // from 0 to 255 to vary duty cycle
   }

void lib_pwm_setduty0B(unsigned char value)
   {
   OCR0B=value;  // from 0 to 255 to vary duty cycle
   }
#endif

#ifdef USEPWM1
void lib_pwm_init1(unsigned char mode,unsigned char output,unsigned char prescaler,unsigned int top)
   {
   OCR1A=0;  // from 0 to top to vary duty cycle
   OCR1B=0;  // from 0 to top to vary duty cycle
   
   if (mode & (1<<0)) TCCR1A |=(1<<WGM10); 
   if (mode & (1<<1)) TCCR1A |=(1<<WGM11); 
   if (mode & (1<<2)) TCCR1B |=(1<<WGM12); 
   if (mode & (1<<3)) TCCR1B |=(1<<WGM13); 

   TCCR1B |=prescaler;

   TCCR1A |=output;
   
   ICR1=top;
   }

void lib_pwm_setduty1A(unsigned int value)
   {
   OCR1A=value;  // from 0 to top to vary duty cycle
   }

void lib_pwm_setduty1B(unsigned int value)
   {
   OCR1B=value;  // from 0 to top to vary duty cycle
   }

#endif

#ifdef USEPWM2
void lib_pwm_init2(unsigned char mode,unsigned char output,unsigned char prescaler)
   {
   OCR2A=0;  // from 0 to 255 to vary duty cycle
   OCR2B=0;  // from 0 to 255 to vary duty cycle
   TCCR2A |=mode | output; // mode for switching on and off
   
   TCCR2B |=prescaler;
   }

void lib_pwm_setduty2A(unsigned char value)
   {
   OCR2A=value;  // from 0 to 255 to vary duty cycle
   }

void lib_pwm_setduty2B(unsigned char value)
   {
   OCR2B=value;  // from 0 to 255 to vary duty cycle
   }
#endif

#ifdef USEPWM3
void lib_pwm_init3(unsigned char mode,unsigned char output,unsigned char prescaler,unsigned int top)
   {
   OCR3A=0;  // from 0 to top to vary duty cycle
   OCR3B=0;  // from 0 to top to vary duty cycle
   
   if (mode & (1<<0)) TCCR3A |=(1<<WGM30); 
   if (mode & (1<<1)) TCCR3A |=(1<<WGM31); 
   if (mode & (1<<2)) TCCR3B |=(1<<WGM32); 
   if (mode & (1<<3)) TCCR3B |=(1<<WGM33); 

   TCCR3B |=prescaler;

   TCCR3A |=output;
   
   ICR3=top;
   }

void lib_pwm_setduty3A(unsigned int value)
   {
   OCR3A=value;  // from 0 to top to vary duty cycle
   }

void lib_pwm_setduty3B(unsigned int value)
   {
   OCR3B=value;  // from 0 to top to vary duty cycle
   }

void lib_pwm_setduty3C(unsigned int value)
   {
   OCR3C=value;  // from 0 to top to vary duty cycle
   }
#endif

#ifdef USEPWM4
void lib_pwm_init4(unsigned char mode,unsigned char output,unsigned char prescaler,unsigned int top)
   {
   OCR4A=0;  // from 0 to top to vary duty cycle
   OCR4B=0;  // from 0 to top to vary duty cycle
   
   if (mode & (1<<0)) TCCR4A |=(1<<WGM40); 
   if (mode & (1<<1)) TCCR4A |=(1<<WGM41); 
   if (mode & (1<<2)) TCCR4B |=(1<<WGM42); 
   if (mode & (1<<3)) TCCR4B |=(1<<WGM43); 
   
   TCCR4A |=output;
   
   TCCR4B |=prescaler;
   
   ICR4=top;
   }

void lib_pwm_setduty4A(unsigned int value)
   {
   OCR4A=value;  // from 0 to top to vary duty cycle
   }

void lib_pwm_setduty4B(unsigned int value)
   {
   OCR4B=value;  // from 0 to top to vary duty cycle
   }

void lib_pwm_setduty4C(unsigned int value)
   {
   OCR4C=value;  // from 0 to top to vary duty cycle
   }
#endif

#ifdef USEPWM411BIT
void lib_pwm_init4(unsigned char mode,unsigned char output,unsigned char prescaler,unsigned int top)
   {
   OCR4A=0;  // from 0 to top to vary duty cycle
   OCR4B=0;  // from 0 to top to vary duty cycle
   OCR4D=0;  // from 0 to top to vary duty cycle

   TCCR4D |=mode; 
   
   TCCR4A |=(output & 0xF0);
   TCCR4C |=(output & 0x0F);
   
   if (output & (PWM411BITNORMALOUTPUTA | PWM411BITNORMALANDINVERTEDOUTPUTA))
      TCCR4A |= (1<<PWM4A); // enable port A
   if (output & (PWM411BITNORMALOUTPUTB | PWM411BITNORMALANDINVERTEDOUTPUTB))
      TCCR4A |= (1<<PWM4B); // enable port B
   if (output & (PWM411BITNORMALOUTPUTD | PWM411BITNORMALANDINVERTEDOUTPUTD))
      TCCR4C |= (1<<PWM4D); // enable port B

   TCCR4B |=prescaler;
   if (top & 0x400)
      TCCR4E |= (1<<ENHC4); // enhanced mode
   TC4H = (top & 0x300)>>8; 
   OCR4C = (top & 0xFF); // phase and frequency correct mode & top to 1023 but with enhanced pwm mode we have 2047   OCR4C=top;
   }

void lib_pwm_setduty4A(unsigned int value)
   {
   TC4H =value>>8;
   OCR4A=value & 0xFF;  // from 0 to top to vary duty cycle
   }

void lib_pwm_setduty4B(unsigned int value)
   {
   TC4H =value>>8;
   OCR4B=value & 0xFF;  // from 0 to top to vary duty cycle
   }

void lib_pwm_setduty4D(unsigned int value)
   {
   TC4H =value>>8;
   OCR4D=value & 0xFF;  // from 0 to top to vary duty cycle
   }
#endif

#ifdef USEPWM5
void lib_pwm_init5(unsigned char mode,unsigned char output,unsigned char prescaler,unsigned int top)
   {
   OCR5A=0;  // from 0 to top to vary duty cycle
   OCR5B=0;  // from 0 to top to vary duty cycle
   
   if (mode & (1<<0)) TCCR5A |=(1<<WGM50); 
   if (mode & (1<<1)) TCCR5A |=(1<<WGM51); 
   if (mode & (1<<2)) TCCR5B |=(1<<WGM52); 
   if (mode & (1<<3)) TCCR5B |=(1<<WGM53); 
   
   TCCR5A |=output;
   
   TCCR5B |=prescaler;
   
   ICR5=top;
   }

void lib_pwm_setduty5A(unsigned int value)
   {
   OCR5A=value;  // from 0 to top to vary duty cycle
   }

void lib_pwm_setduty5B(unsigned int value)
   {
   OCR5B=value;  // from 0 to top to vary duty cycle
   }

void lib_pwm_setduty5C(unsigned int value)
   {
   OCR5C=value;  // from 0 to top to vary duty cycle
   }
#endif
