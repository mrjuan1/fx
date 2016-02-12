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

#ifndef __GL_H__
#define __GL_H__

#include "main.h"

#include <GLES2/gl2.h>

typedef enum {
	tf_none,
	tf_linear,
	tf_mipmap
} tex_filter;

typedef struct {
	unint texs[2];
	unshort w, h;
	unint id;
} fbo_data;

#define clcol() glClear(GL_COLOR_BUFFER_BIT)
#define cldep() glClear(GL_DEPTH_BUFFER_BIT)
#define use_tex(t) glBindTexture(GL_TEXTURE_2D,t)
#define unuse_fbo() glBindFramebuffer(GL_FRAMEBUFFER,0)

void clear(void);

byte load_pro(unint p, const char *vs_fname, const char *fs_fname);
byte load_tex(unint t, const char *fname, unshort w, unshort h, GLenum fmt, tex_filter tf);

void filter_tex(tex_filter tf);

void init_fbo(fbo_data *f, unshort w, unshort h, tex_filter tf);
void done_fbo(fbo_data *f);

void use_fbo(const fbo_data *f);

#endif /* __GL_H__ */
