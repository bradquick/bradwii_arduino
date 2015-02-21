//
//  lib_prg_mem.h
//  Project
//
//  Created by Brad on 2/14/15.
//
//

#pragma once

#include <avr/pgmspace.h>

#define PROGRAMCHAR const char PROGMEM
#define PROGRAMUCHAR const unsigned char PROGMEM

unsigned char lib_prg_mem_getuchar(PROGRAMUCHAR *prgmemptr);

char lib_prg_mem_getchar(PROGRAMCHAR *prgmemptr);