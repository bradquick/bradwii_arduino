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


// a fixedpointnum is a real number that's stored in a long that's shifted left FIXEDPOINTSHIFT bits
// FIXEDPOINTSHIFT is currently 16, so 16 bits are used for the integer part of the number and
// the other 16 bits are used for the fractional part of the number.  I've tried using different
// values, but the compliler likes shifting things by 16 bits (words at a time) so things run
// faster with a FIXEDPOINTSHIFT of 16.

// Therefore, the range of a fixedpointnum is -32768.0 to 32786.0 with an accuracy of 0.000015
// There is no overflow or underflow protection, so it's up to the programmer to watch out.

#define fixedpointnum long

#define FIXEDPOINTSHIFT 16

#define FIXEDPOINTONE (1L<<FIXEDPOINTSHIFT)

#define FIXEDPOINTCONSTANT(number) ((fixedpointnum)(number*FIXEDPOINTONE))

#define FIXEDPOINT45 (45L<<FIXEDPOINTSHIFT)
#define FIXEDPOINT80 (80L<<FIXEDPOINTSHIFT)
#define FIXEDPOINT90 (90L<<FIXEDPOINTSHIFT)
#define FIXEDPOINT135 (135L<<FIXEDPOINTSHIFT)
#define FIXEDPOINT180 (180L<<FIXEDPOINTSHIFT)
#define FIXEDPOINT360 (360L<<FIXEDPOINTSHIFT)
#define FIXEDPOINTTWO (2L<<FIXEDPOINTSHIFT)
#define FIXEDPOINTTHREE (3L<<FIXEDPOINTSHIFT)
#define FIXEDPOINTONEOVERONE FIXEDPOINTONE
#define FIXEDPOINTONEOVERONEHALF FIXEDPOINTTWO
#define FIXEDPOINTONEOVERTWO (FIXEDPOINTONE>>1)
#define FIXEDPOINTONEOVERFOUR (FIXEDPOINTONE>>2)
#define FIXEDPOINTONEOVERONEFOURTH (FIXEDPOINTONE<<2)
#define FIXEDPOINTONEOVERONESIXTEENTH (FIXEDPOINTONE<<4)
#define FIXEDPOINTONEOVERONESIXTYITH (60L<<FIXEDPOINTSHIFT)
#define FIXEDPOINTONEFIFTIETH ((1L<<FIXEDPOINTSHIFT)/50)

#define FIXEDPOINTPIOVER180 1144L // pi/180 for converting degrees to radians

// since time slivers can be very small, we shift them an extra 8 bits to maintain accuracy
#define TIMESLIVEREXTRASHIFT 8

void lib_fp_constrain(fixedpointnum *lf,fixedpointnum low,fixedpointnum high);
void lib_fp_constrain180(fixedpointnum *lf);
fixedpointnum lib_fp_multiply(fixedpointnum x,fixedpointnum y);

void lib_fp_lowpassfilter(fixedpointnum *variable,fixedpointnum newvalue,fixedpointnum timesliver,fixedpointnum oneoverperiod,int timesliverextrashift);
fixedpointnum lib_fp_abs(fixedpointnum fp);
fixedpointnum lib_fp_sine(fixedpointnum angle);
fixedpointnum lib_fp_cosine(fixedpointnum angle);
fixedpointnum lib_fp_atan2(fixedpointnum y, fixedpointnum x);
fixedpointnum lib_fp_sqrt(fixedpointnum x);
fixedpointnum lib_fp_stringtofixedpointnum(char *string);
fixedpointnum lib_fp_invsqrt(fixedpointnum x);
long lib_fp_stringtolong(char *string);