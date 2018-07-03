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

const GLenum _clbit=(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
const GLenum _drawbufs[16]={ /* is this okay? */
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,
	GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6,
	GL_COLOR_ATTACHMENT7,
	GL_COLOR_ATTACHMENT8,
	GL_COLOR_ATTACHMENT9,
	GL_COLOR_ATTACHMENT10,
	GL_COLOR_ATTACHMENT11,
	GL_COLOR_ATTACHMENT12,
	GL_COLOR_ATTACHMENT13,
	GL_COLOR_ATTACHMENT14,
	GL_COLOR_ATTACHMENT15
};

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

byte load_tex_compressed(unint t, const char *fname, tex_filter tf)
{
	int size=0; char *data=NULL;
	if(fload(fname,&size,(void**)&data))
	{
		GLenum fmt=GL_COMPRESSED_RGB8_ETC2;
		int offset=0; byte mips=1, i=0;

		memcpy((void*)&fmt,(const void*)data,sizeof(GLenum)); offset+=sizeof(GLenum);
		if(tf==tf_mipmap) memcpy((void*)&mips,(const void*)(data+offset),sizeof(byte));
		offset+=sizeof(byte);

		use_tex(t);

		for(i=0 ; i<mips ; i++)
		{
			unshort w=0, h=0;

			memcpy((void*)&w,(const void*)(data+offset),sizeof(unshort)); offset+=sizeof(unshort);
			memcpy((void*)&h,(const void*)(data+offset),sizeof(unshort)); offset+=sizeof(unshort);
			memcpy((void*)&size,(const void*)(data+offset),sizeof(int)); offset+=sizeof(int);

			glCompressedTexImage2D(GL_TEXTURE_2D,i,fmt,w,h,0,size,(const void*)(data+offset));
			offset+=size;
		}

		if(mips>1)
		{
			if(_anis==-1)
			{
				glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&_anis);
				if(_anis==0) info("Anisotropic texture filtering not supported\n");
			}
			if(_anis>0) glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,_anis);

			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

			clamp_tex(1);
		}
		else filter_tex(tf);

		free((void*)data);

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
			if(_anis==-1)
			{
				glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&_anis);
				if(_anis==0) info("Anisotropic texture filtering not supported\n");
			}
			if(_anis>0) glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,_anis);

			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
	}

	clamp_tex(1);
}

void clamp_tex(byte on)
{
	if(on)
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	}
}

void add_fb_rb(int attachment, unint r, unshort w, unshort h, GLenum fmt, int samples)
{
	GLenum a=GL_COLOR_ATTACHMENT0;

	use_rb(r);
	if(samples>0)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER,samples,fmt,w,h);
	else glRenderbufferStorage(GL_RENDERBUFFER,fmt,w,h);

	switch(fmt)
	{
		case GL_DEPTH_COMPONENT:
		case GL_DEPTH_COMPONENT16:
		case GL_DEPTH_COMPONENT24:
		case GL_DEPTH_COMPONENT32F:
			a=GL_DEPTH_ATTACHMENT;
			break;
		default:
			a+=(GLenum)attachment;
			/*if(a>GL_MAX_COLOR_ATTACHMENTS)
				info("Renderbuffer attachment exceeds max allowed\n");*/
	}

	glFramebufferRenderbuffer(GL_FRAMEBUFFER,a,GL_RENDERBUFFER,r);
}

void add_fb_tex(int attachment, unint t, unshort w, unshort h, GLint ifmt, tex_filter tf)
{
	GLenum fmt=GL_RGB;
	GLenum type=GL_UNSIGNED_BYTE;
	GLenum a=GL_COLOR_ATTACHMENT0;

	use_tex(t);
	filter_tex(tf);

	switch(ifmt)
	{
		case GL_RG8: fmt=GL_RG; break;
		case GL_RGBA8: fmt=GL_RGBA; break;
		case GL_RG16F: case GL_RG32F: fmt=GL_RG; type=GL_FLOAT; break;
		case GL_RGB16F: case GL_RGB32F: type=GL_FLOAT; break;
		case GL_RGBA16F: case GL_RGBA32F: fmt=GL_RGBA; type=GL_FLOAT; break;
		case GL_DEPTH_COMPONENT16: fmt=GL_DEPTH_COMPONENT; type=GL_UNSIGNED_SHORT; break;
		case GL_DEPTH_COMPONENT24: fmt=GL_DEPTH_COMPONENT; type=GL_UNSIGNED_INT; break;
		case GL_DEPTH_COMPONENT32F: fmt=GL_DEPTH_COMPONENT; type=GL_FLOAT;
	}
	glTexImage2D(GL_TEXTURE_2D,0,ifmt,w,h,0,fmt,type,NULL);

	if(fmt==GL_DEPTH_COMPONENT)
		a=GL_DEPTH_ATTACHMENT;
	else
	{
		a+=(GLenum)attachment;
		/*if(a>GL_MAX_COLOR_ATTACHMENTS)
			info("Texture attachment exceeds max allowed\n");*/
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER,a,GL_TEXTURE_2D,t,0);
}

void set_drawbufs(int count)
{
	glDrawBuffers(count,_drawbufs); /* is this okay? */
}

void active_tex(GLenum texnum, unint t)
{
	glActiveTexture(texnum);
	use_tex(t);
}

void blit_fb(unint from, unint to, unshort w, unshort h)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER,from);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,to);
	glBlitFramebuffer(0,0,w,h,0,0,w,h,_clbit,GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER,0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
}
