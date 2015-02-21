//
//  lib_prg_mem.cpp
//  Project
//
//  Created by Brad on 2/14/15.
//
//

#include "lib_prg_mem.h"
#include <avr/pgmspace.h>

unsigned char lib_prg_mem_getuchar(PROGRAMUCHAR *prgmemptr)
   {
   return(pgm_read_byte(prgmemptr));
   }

char lib_prg_mem_getchar(PROGRAMCHAR *prgmemptr)
   {
   return(pgm_read_byte(prgmemptr));
   }

