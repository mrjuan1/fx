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

#include "main.h"

#include <stdarg.h>
#include <stdio.h>

#include <signal.h>
#include "SDL.h"
#include <GLES3/gl3.h>
#include <time.h>

int _sw=0, _sh=0;
float _asp=0.0f;

byte _ki[SDL_NUM_SCANCODES];
byte _kp[SDL_NUM_SCANCODES];
byte _mi[3], _mp[3], _mm=1;
float _mx=0.0f, _my=0.0f, _mw=0.0f;
float _dt=0.0f;

byte running=1, active=1;

void sigquit(int sig);

int main(void)
{
	SDL_Window *win=NULL;
	SDL_GLContext context;

	int start=0, lt=0;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,0);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	/* Place window at bottom-right of main screen. Suits my development environment/layout, can be changed as needed */
	win=SDL_CreateWindow(TITLE,1920-SW,1080-SH,SW,SH,SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);

	/* Incomment to capture the mouse on startup */
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_ShowCursor(0);

	context=SDL_GL_CreateContext(win);
	if(SDL_GL_SetSwapInterval(-1)==-1 && SDL_GL_SetSwapInterval(1)==-1)
		info("Vsync not supported");
	SDL_GL_GetDrawableSize(win,&_sw,&_sh);
	_asp=(float)_sw/(float)_sh;

	glViewport(0,0,_sw,_sh);
	glScissor(0,0,_sw,_sh);
	glEnable(GL_SCISSOR_TEST);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClearDepthf(1.0f);

	srand(time(NULL));
	init();
	start=SDL_GetTicks();

	while(running)
	{
		SDL_Event event;
		int t=0;

		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_WINDOWEVENT:
					switch(event.window.event)
					{
						case SDL_WINDOWEVENT_FOCUS_GAINED: active=1; break;
						case SDL_WINDOWEVENT_FOCUS_LOST: active=0;
					}
					break;
				case SDL_KEYDOWN: _ki[event.key.keysym.scancode]=1; break;
				case SDL_KEYUP: _ki[event.key.keysym.scancode]=0; break;
				case SDL_MOUSEBUTTONDOWN: _mi[event.button.button]=1; break;
				case SDL_MOUSEBUTTONUP: _mi[event.button.button]=0; break;
				case SDL_MOUSEMOTION:
					_mm=1;
					_mx=(float)event.motion.xrel;
					_my=(float)event.motion.yrel;
					break;
				case SDL_MOUSEWHEEL: _mw=(float)event.wheel.y; break;
				case SDL_QUIT: running=0;
			}
		}

		if(active)
		{
			t=SDL_GetTicks()-start;
			_dt=(float)(t-lt)/1000.0f;
			lt=t;

			loop();
			SDL_GL_SwapWindow(win);

			SDL_Delay(10);
		}
		else
		{
			start=SDL_GetTicks();
			lt=0;

			/* Uncomment and remove the temp line to not quit when the window looses focus */
			/* This is just intended as a quick way to quit when I'm done testing and wish to return to coding quickly */
			/*SDL_Delay(100);*/
			running=0; /* temp */
		}

		_mm=0;
		_mx=_my=_mw=0.0f;
	}

	done();

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}

void sigquit(int sig)
{
	switch(sig)
	{
		case SIGINT: case SIGTERM:
			running=0;
	}
}

void info(const char *text, ...)
{
	va_list args;
	va_start(args,text);

	vprintf(text,args);
	fflush(stdout);

	va_end(args);
}

byte fload(const char *fname, int *size, void **data)
{
	SDL_RWops *f=NULL;

	if((f=SDL_RWFromFile(fname,"rb")))
	{
		*size=SDL_RWsize(f);
		*data=malloc(*size);
		SDL_RWread(f,*data,*size,1);
		SDL_RWclose(f);

		return 1;
	}
	else
	{
		info("File \"%s\" could not be loaded\n",fname);
		return 0;
	}
}

int get_sw(void)
{
	return _sw;
}

int get_sh(void)
{
	return _sh;
}

float asp(void)
{
	return _asp;
}

byte ki(unshort k)
{
	return _ki[k];
}

byte kp(unshort k)
{
	if(_ki[k] && !_kp[k]) return (_kp[k]=1);
	else if(!_ki[k] && _kp[k]) _kp[k]=0;

	return 0;
}

byte mi(byte m)
{
	return _mi[m];
}

byte mp(byte m)
{
	if(_mi[m] && !_mp[m]) return (_mp[m]=1);
	else if(!_mi[m] && _mp[m]) _mp[m]=0;

	return 0;
}

byte get_mm(void)
{
	return _mm;
}

float get_mx(void)
{
	return _mx;
}

float get_my(void)
{
	return _my;
}

float get_mw(void)
{
	return _mw;
}

float get_dt(void)
{
	return _dt;
}

void quit(void)
{
	running=0;
}
