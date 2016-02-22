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

#include "gl.h"

#include <GLES2/gl2ext.h>

const GLenum _clbit=(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

int _anis=-1;

unint load_shader(const char *fname, GLenum type);

unint load_shader(const char *fname, GLenum type)
{
	int size=0; void *data=NULL;
	if(fload(fname,&size,&data))
	{
		const unint s=glCreateShader(type);
		int val=0;

		glShaderSource(s,1,(const char**)&data,&size);
		free(data);

		glCompileShader(s);
		glGetShaderiv(s,GL_INFO_LOG_LENGTH,&val);
		if(val>1)
		{
			char *log=(char*)malloc(val);
			glGetShaderInfoLog(s,val,NULL,log);
			info("Shader \"%s\" info:\n%s\n",fname,log);
			free((void*)log);
		}

		glGetShaderiv(s,GL_COMPILE_STATUS,&val);
		if(!val) info("Shader \"%s\" compiling failed\n",fname);

		return s;
	}
	else return 0;
}

void viewport(unshort x, unshort y, unshort w, unshort h)
{
	glViewport(x,y,w,h);
	glScissor(x,y,w,h);
}

void clear(void)
{
	glClear(_clbit);
}

byte load_pro(unint p, const char *vs_fname, const char *fs_fname)
{
	const unint vs=load_shader(vs_fname,GL_VERTEX_SHADER);
	const unint fs=load_shader(fs_fname,GL_FRAGMENT_SHADER);
	int val=0;

	glAttachShader(p,vs);
	glAttachShader(p,fs);
	glLinkProgram(p);
	glDetachShader(p,vs);
	glDetachShader(p,fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	glGetProgramiv(p,GL_INFO_LOG_LENGTH,&val);
	if(val>1)
	{
		char *log=(char*)malloc(val);
		glGetProgramInfoLog(p,val,NULL,log);
		info("Program info:\n%s\n",log);
		free((void*)log);
	}

	glGetProgramiv(p,GL_LINK_STATUS,&val);
	if(!val) info("Program linking failed\n");

	return (byte)val;
}

byte load_tex(unint t, const char *fname, unshort w, unshort h, GLenum fmt, tex_filter tf)
{
	int size=0; void *data=NULL;
	if(fload(fname,&size,&data))
	{
		use_tex(t);
		glTexImage2D(GL_TEXTURE_2D,0,fmt,w,h,0,fmt,GL_UNSIGNED_BYTE,data);
		free(data);
		filter_tex(tf);

		return 1;
	}
	else return 0;
}

void filter_tex(tex_filter tf)
{
	switch(tf)
	{
		case tf_none:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			break;
		case tf_linear:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			break;
		case tf_mipmap:
			if(_anis==-1) glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&_anis);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,_anis);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
	}

	wrap_tex(tw_clamp);
}

void wrap_tex(tex_wrap tw)
{
	switch(tw)
	{
		case tw_clamp:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
			break;
		case tw_repeat:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	}
}

/* rework fbo support (see todo.txt) */
