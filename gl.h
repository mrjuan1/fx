/* Copyright (c) 2016 Juan Wolfaardt

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

#include "glad.h"

typedef enum { tf_none, tf_linear, tf_mipmap } tex_filter;

#define clcol() glClear(GL_COLOR_BUFFER_BIT)
#define cldep() glClear(GL_DEPTH_BUFFER_BIT)

#define gen_fbs glGenFramebuffers
#define gen_rbs glGenRenderbuffers
#define gen_texs glGenTextures

#define del_fbs glDeleteFramebuffers
#define del_rbs glDeleteRenderbuffers
#define del_texs glDeleteTextures

#define use_fb(f) glBindFramebuffer(GL_FRAMEBUFFER, f)
#define use_rb(r) glBindRenderbuffer(GL_RENDERBUFFER, r)
#define use_tex(t) glBindTexture(GL_TEXTURE_2D, t)

#define default_tex() glActiveTexture(GL_TEXTURE0)

void viewport(unshort x, unshort y, unshort w, unshort h);
void clear(void);

byte load_pro(unint p, const char *vs_fname, const char *fs_fname);

byte load_tex(unint t, const char *fname, unshort w, unshort h, GLenum fmt,
              tex_filter tf);
byte load_tex_compressed(unint t, const char *fname, tex_filter tf);
void filter_tex(tex_filter tf);
void clamp_tex(byte on);

void add_fb_rb(int attachment, unint r, unshort w, unshort h, GLenum fmt,
               int samples);
void add_fb_tex(int attachment, unint t, unshort w, unshort h, GLint ifmt,
                tex_filter tf);

void set_drawbufs(int count);
void active_tex(GLenum texnum, unint t);

void blit_fb(unint from, unint to, unshort w, unshort h);

#endif /* __GL_H__ */
