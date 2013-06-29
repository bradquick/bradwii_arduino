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

// for timer 0: (8 bit)
// mode
#define PWM0FASTMODE (1<<WGM02) | (1<<WGM00)
#define PWM0PHASECORRECTMODE (1<<WGM00)

// output
#define PWM0NORMALOUTPUTA (1<<COM0A1)
#define PWM0INVERTEDOUTPUTA (1<<COM0A1) | (1<<COM0A0)

#define PWM0NORMALOUTPUTB (1<<COM0B1)
#define PWM0INVERTEDOUTPUTB (1<<COM0B1) | (1<<COM0B0)

// prescaler
#define PWM0PRESCALER1 (1<<CS00)
#define PWM0PRESCALER8 (1<<CS01)
#define PWM0PRESCALER64 (1<<CS01) | (1<<CS00)
#define PWM0PRESCALER256 (1<<CS02)
#define PWM0PRESCALER1024 (1<<CS02) | (1<<CS00)

// for timer 1: (16bit)
// mode
#define PWM1FASTMODE 14
#define PWM1PHASECORRECTMODE 10

// output
#define PWM1NORMALOUTPUTA (1<<COM1A1)
#define PWM1INVERTEDOUTPUTA (1<<COM1A1) | (1<<COM1A0)

#define PWM1NORMALOUTPUTB (1<<COM1B1)
#define PWM1INVERTEDOUTPUTB (1<<COM1B1) | (1<<COM1B0)

// prescaler
#define PWM1PRESCALER1 (1<<CS10)
#define PWM1PRESCALER8 (1<<CS11)
#define PWM1PRESCALER64 (1<<CS11) | (1<<CS10)
#define PWM1PRESCALER256 (1<<CS12)
#define PWM1PRESCALER1024 (1<<C12) | (1<<CS10)

// for timer 2: (8 bit)
// mode
#define PWM2FASTMODE (1<<WGM22) | (1<<WGM20)
#define PWM2PHASECORRECTMODE (1<<WGM20)

// output
#define PWM2NORMALOUTPUTA (1<<COM2A1)
#define PWM2INVERTEDOUTPUTA (1<<COM2A1) | (1<<COM2A0)

#define PWM2NORMALOUTPUTB (1<<COM2B1)
#define PWM2INVERTEDOUTPUTB (1<<COM2B1) | (1<<COM2B0)

// prescaler
#define PWM2PRESCALER1 (1<<CS20)
#define PWM2PRESCALER8 (1<<CS21)
#define PWM2PRESCALER32 (1<<CS21) | (1<<CS20)
#define PWM2PRESCALER64 (1<<CS22)
#define PWM2PRESCALER128 (1<<CS22) | (1<<CS20)
#define PWM2PRESCALER256 (1<<CS22) | (1<<CS21)
#define PWM2PRESCALER1024 (1<<CS22) | (1<<CS21) | (1<<CS20)

// for timer 3: (16bit)
// mode
#define PWM3FASTMODE 14
#define PWM3PHASECORRECTMODE 10

// output
#define PWM3NORMALOUTPUTA (1<<COM3A1)
#define PWM3INVERTEDOUTPUTA (1<<COM3A1) | (1<<COM3A0)

#define PWM3NORMALOUTPUTB (1<<COM3B1)
#define PWM3INVERTEDOUTPUTB (1<<COM3B1) | (1<<COM3B0)

#define PWM3NORMALOUTPUTC (1<<COM3C1)
#define PWM3INVERTEDOUTPUTC (1<<COM3C1) | (1<<COM3C0)

// prescaler
#define PWM3PRESCALER1 (1<<CS30)
#define PWM3PRESCALER8 (1<<CS31)
#define PWM3PRESCALER64 (1<<CS31) | (1<<CS30)
#define PWM3PRESCALER256 (1<<CS32)
#define PWM3PRESCALER1024 (1<<C32) | (1<<CS30)

// for timer 4: (16bit)
// mode
#define PWM4FASTMODE 14
#define PWM4PHASECORRECTMODE 10

// output
#define PWM4NORMALOUTPUTA (1<<COM4A1)
#define PWM4INVERTEDOUTPUTA (1<<COM4A1) | (1<<COM4A0)

#define PWM4NORMALOUTPUTB (1<<COM4B1)
#define PWM4INVERTEDOUTPUTB (1<<COM4B1) | (1<<COM4B0)

#define PWM4NORMALOUTPUTC (1<<COM4C1)
#define PWM4INVERTEDOUTPUTC (1<<COM4C1) | (1<<COM4C0)

// prescaler
#define PWM4PRESCALER1 (1<<CS40)
#define PWM4PRESCALER8 (1<<CS41)
#define PWM4PRESCALER64 (1<<CS41) | (1<<CS40)
#define PWM4PRESCALER256 (1<<CS42)
#define PWM4PRESCALER1024 (1<<CS42) | (1<<CS40)

// for 11 bit timer 4: (11bit)
// mode
#define PWM411BITFASTMODE 0
#define PWM411BITPHASECORRECTMODE (1<<WGM40)

// output
#define PWM411BITNORMALOUTPUTA (1<<COM4A1)
#define PWM411BITNORMALANDINVERTEDOUTPUTA (1<<COM4A0)

#define PWM411BITNORMALOUTPUTB (1<<COM4B1)
#define PWM411BITNORMALANDINVERTEDOUTPUTB (1<<COM4B0)

#define PWM411BITNORMALOUTPUTD (1<<COM4D1)
#define PWM411BITNORMALANDINVERTEDOUTPUTD (1<<COM4D0)

// prescaler
#define PWM411BITPRESCALER1 (1<<CS40)
#define PWM411BITPRESCALER2 (1<<CS41)
#define PWM411BITPRESCALER4 (1<<CS41) | (1<<CS40)
#define PWM411BITPRESCALER8 (1<<CS42)
#define PWM411BITPRESCALER16 (1<<CS42) | (1<<CS40)
#define PWM411BITPRESCALER32 (1<<CS42) | (1<<CS40)
#define PWM411BITPRESCALER64 (1<<CS42) | (1<<CS41) | (1<<CS40)
#define PWM411BITPRESCALER128 (1<<CS43)
#define PWM411BITPRESCALER256 (1<<CS43) | (1<<CS40)
#define PWM411BITPRESCALER512 (1<<CS43) | (1<<CS41)
#define PWM411BITPRESCALER1024 (1<<CS43) | (1<<CS41) | (1<<CS40)
#define PWM411BITPRESCALER2048 (1<<CS43) | (1<<CS42)
#define PWM411BITPRESCALER4096 (1<<CS43) | (1<<CS42) | (1<<CS40)
#define PWM411BITPRESCALER8192 (1<<CS43) | (1<<CS42)| (1<<CS41)
#define PWM411BITPRESCALER16384 (1<<CS43) | (1<<CS42) | (1<<CS41) | (1<<CS40)

// for timer 5: (16bit)
// mode
#define PWM5FASTMODE 14
#define PWM5PHASECORRECTMODE 10

// output
#define PWM5NORMALOUTPUTA (1<<COM5A1)
#define PWM5INVERTEDOUTPUTA (1<<COM5A1) | (1<<COM5A0)

#define PWM5NORMALOUTPUTB (1<<COM5B1)
#define PWM5INVERTEDOUTPUTB (1<<COM5B1) | (1<<COM5B0)

#define PWM5NORMALOUTPUTC (1<<COM5C1)
#define PWM5INVERTEDOUTPUTC (1<<COM5C1) | (1<<COM5C0)

// prescaler
#define PWM5PRESCALER1 (1<<CS50)
#define PWM5PRESCALER8 (1<<CS51)
#define PWM5PRESCALER64 (1<<CS51) | (1<<CS50)
#define PWM5PRESCALER256 (1<<CS52)
#define PWM5PRESCALER1024 (1<<C52) | (1<<CS50)

void lib_pwm_init0(unsigned char mode,unsigned char output,unsigned char prescaler);
void lib_pwm_setduty0A(unsigned char value);
void lib_pwm_setduty0B(unsigned char value);
void lib_pwm_init1(unsigned char mode,unsigned char output,unsigned char prescaler,unsigned int top);
void lib_pwm_setduty1A(unsigned int value);
void lib_pwm_setduty1B(unsigned int value);
void lib_pwm_init2(unsigned char mode,unsigned char output,unsigned char prescaler);
void lib_pwm_setduty2A(unsigned char value);
void lib_pwm_setduty2B(unsigned char value);
void lib_pwm_init3(unsigned char mode,unsigned char output,unsigned char prescaler,unsigned int top);
void lib_pwm_setduty3A(unsigned int value);
void lib_pwm_setduty3B(unsigned int value);
void lib_pwm_setduty3C(unsigned int value);
void lib_pwm_init4(unsigned char mode,unsigned char output,unsigned char prescaler,unsigned int top);
void lib_pwm_setduty4A(unsigned int value);
void lib_pwm_setduty4B(unsigned int value);
void lib_pwm_setduty4C(unsigned int value);
void lib_pwm_setduty4D(unsigned int value);
void lib_pwm_init5(unsigned char mode,unsigned char output,unsigned char prescaler,unsigned int top);
void lib_pwm_setduty5A(unsigned int value);
void lib_pwm_setduty5B(unsigned int value);
void lib_pwm_setduty5C(unsigned int value);
