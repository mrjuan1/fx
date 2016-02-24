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

#include "app.h"

const GLenum fb0bufs[2]={
	GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1
}, fb1bufs[1]={
	GL_COLOR_ATTACHMENT0
};

mat4 proj, view, pview;
float dir=180.0f, tilt=90.0f, zoom=10.0f;
float ndir=180.0f, ntilt=90.0f, nzoom=10.0f;

vbo_data area_mod;
unint area_tex=0;

int nsw=0, nsh=0;
unint texs[4], rbs[3];
unint fbos[4];

void init(void)
{
	if(init_mblur() && init_vig() && init_quad())
	{
		int size=0; void *data=NULL;

		/*glClearColor(1.0f,1.0f,1.0f,1.0f);*/
		glClearColor(0.04f,0.0f,0.04f,1.0f);

		use_mblur();
		send_mblur_samples(16.0f); /* ----- */

		use_vig();
		send_vig_size((float)sw,(float)sh);
		vigmode(0);

		use_basic();
		persp(proj,75.0f,asp(),0.1f,24.0f);

		if(fload("data/area.bin",&size,&data))
		{
			add_to_vbo(&area_mod,data,size);
			free(data);

			glGenTextures(1,&area_tex);
			/*if(load_tex(area_tex,"data/area-ao.data",2048,2048,GL_LUMINANCE,tf_mipmap))*/
			if(load_tex(area_tex,"data/area.data",2048,2048,GL_RGB,tf_mipmap))
			{
				update_vbo();
				texmode(1);

				nsw=sw/1;
				nsh=sh/1;

				glGenTextures(4,texs);
				use_tex(texs[0]);
				filter_tex(tf_linear);
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,nsw,nsh,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
				use_tex(texs[1]);
				filter_tex(tf_linear);
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,nsw,nsh,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
				use_tex(texs[2]);
				filter_tex(tf_linear);
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,nsw,nsh,0,GL_RGBA,GL_FLOAT,NULL);
				use_tex(texs[3]);
				filter_tex(tf_linear);
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,nsw,nsh,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
				use_tex(0);

				glGenRenderbuffers(3,rbs);
				glBindRenderbuffer(GL_RENDERBUFFER,rbs[0]);
				glRenderbufferStorageMultisample(GL_RENDERBUFFER,8,GL_RGB8,nsw,nsh); /* adjust renderbuffer samples here */
				glBindRenderbuffer(GL_RENDERBUFFER,rbs[1]);
				glRenderbufferStorageMultisample(GL_RENDERBUFFER,8,GL_DEPTH_COMPONENT24,nsw,nsh); /* adjust renderbuffer samples here */
				glBindRenderbuffer(GL_RENDERBUFFER,rbs[2]);
				glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,nsw,nsh);
				glBindRenderbuffer(GL_RENDERBUFFER,0);

				glGenFramebuffers(4,fbos);
				glBindFramebuffer(GL_FRAMEBUFFER,fbos[0]);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_RENDERBUFFER,rbs[0]);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rbs[1]);
				glBindFramebuffer(GL_FRAMEBUFFER,fbos[1]);
				glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texs[0],0);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rbs[2]);
				glBindFramebuffer(GL_FRAMEBUFFER,fbos[2]);
				glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texs[1],0);
				glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,texs[2],0);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rbs[2]);
				glBindFramebuffer(GL_FRAMEBUFFER,fbos[3]);
				glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texs[3],0);
				unuse_fbo();
			}
			else quit();
		}
		else quit();
	}
	else quit();
}

void loop(void)
{
	const float spd1=30.0f*dt;
	const float spd2=15.0f*dt;

	float r=0.0f;
	float dc=0.0f, ds=0.0f;
	float tc=0.0f, ts=0.0f;

	/*viewport(0,0,nsw,nsh);*/
	use_basic();
	send_lpview(pview);

	if(mm || mw!=0.0f)
	{
		if(mm)
		{
			dir+=mx*spd1;
			if(dir<-0.0f)
			{
				dir+=360.0f;
				ndir+=360.0f;
			}
			else if(dir>360.0f)
			{
				dir-=360.0f;
				ndir-=360.0f;
			}

			tilt-=my*spd1;
			if(tilt<0.001f) tilt=0.001f;
			else if(tilt>100.0f) tilt=100.0f;
		}

		if(mw!=0.0f)
		{
			zoom-=mw;
			if(zoom<7.0f) zoom=7.0f;
			else if(zoom>12.0f) zoom=12.0f;
		}
	}

	ndir-=(ndir-dir)*spd2;
	r=d2r(ndir);
	dc=cosf(r);
	ds=sinf(r);

	ntilt-=(ntilt-tilt)*spd2;
	r=d2r(ntilt);
	tc=cosf(r);
	ts=sinf(r);

	nzoom-=(nzoom-zoom)*spd2;

	look(view,v3(ds*ts*nzoom,dc*ts*nzoom,3.0f+(tc*nzoom)),v3(0.0f,0.0f,3.0f));
	mult(pview,proj,view);

	send_pview(pview);

	/* ----- */
	glBindFramebuffer(GL_FRAMEBUFFER,fbos[0]);
	clear();
	/* ----- */
	use_tex(area_tex);
	/* ----- */
	draw_vbo(&area_mod);
	unuse_fbo();

	glBindFramebuffer(GL_READ_FRAMEBUFFER,fbos[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fbos[1]);
	glBlitFramebuffer(0,0,sw,sh,0,0,sw,sh,GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT,GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER,0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
	/* ----- */

	glBindFramebuffer(GL_FRAMEBUFFER,fbos[2]);
	glDrawBuffers(2,fb0bufs);
	clear();
	draw_vbo(&area_mod);
	unuse_fbo();

	use_mblur();
	use_tex(texs[0]); /* ----- */
	glActiveTexture(GL_TEXTURE1);
	use_tex(texs[2]);
	glActiveTexture(GL_TEXTURE0);

	glBindFramebuffer(GL_FRAMEBUFFER,fbos[3]);
	glDrawBuffers(1,fb1bufs);
	clear();
	quad();
	unuse_fbo();

	/*viewport(0,0,sw,sh);*/
	use_vig();
	use_tex(texs[3]);
	clear();
	quad();

	if(kp(SDL_SCANCODE_DELETE) || kp(SDL_SCANCODE_ESCAPE))
		quit();
}

void done(void)
{
	glDeleteFramebuffers(4,fbos);
	glDeleteRenderbuffers(3,rbs);
	glDeleteTextures(4,texs);

	glDeleteTextures(1,&area_tex);

	done_quad();
	done_vbo();

	done_vig();
	done_mblur();
	done_basic();
}
