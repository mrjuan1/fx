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

#ifndef __BASIC_H__
#define __BASIC_H__

#include "app.h"

byte init_basic(void);
void done_basic(void);

void use_basic(void);

void send_pview(const mat4 mat);
void send_lpview(const mat4 mat);
void send_attribs(void);

void send_col(vec4 col);
void texmode(byte on);

#endif /* __BASIC_H__ */
