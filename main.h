/* Copyright (c) 2016 youka

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution. */

#ifndef __MAIN_H__
#define __MAIN_H__

#include <stddef.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "SDL_scancode.h"

typedef unsigned char byte;
typedef unsigned short unshort;
typedef unsigned int unint;

#define sw get_sw()
#define sh get_sh()
#define mm get_mm()
#define mx get_mx()
#define my get_my()
#define mw get_mw()
#define dt get_dt()

void info(const char *text, ...);
byte fload(const char *fname, int *size, void **data);

int get_sw(void), get_sh(void);
float asp(void);

byte ki(unshort k), kp(unshort k);
byte mi(byte m), mp(byte m), get_mm(void);
float get_mx(void), get_my(void), get_mw(void);
float get_dt(void);

void quit(void);

void init(void), loop(void), done(void);

#endif /* __MAIN_H__ */
