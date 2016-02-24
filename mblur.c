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

#include "mblur.h"

byte _mblur_init=0;

unint _mblur_p=0;
struct {
	int colmap, velmap;
	int samples;
} _mblur_pv;

byte init_mblur(void)
{
	if(!_mblur_init && init_basic())
	{
		_mblur_p=glCreateProgram();
		if(load_pro(_mblur_p,"sqvs.glsl","mblur.glsl"))
		{
			use_mblur();

			_mblur_pv.colmap=glGetUniformLocation(_mblur_p,"colmap");
			_mblur_pv.velmap=glGetUniformLocation(_mblur_p,"velmap");

			_mblur_pv.samples=glGetUniformLocation(_mblur_p,"samples");

			glUniform1i(_mblur_pv.colmap,0);
			glUniform1i(_mblur_pv.velmap,1);

			send_mblur_samples(1.0f);

			_mblur_init=1;
		}
		else
		{
			glDeleteProgram(_mblur_p);
			info("Program \"mblur\" not created\n");
		}
	}

	return _mblur_init;
}

void done_mblur(void)
{
	if(_mblur_init)
	{
		glDeleteProgram(_mblur_p);
		_mblur_init=0;
	}
}

void use_mblur(void)
{
	glUseProgram(_mblur_p);
}

void send_mblur_samples(float samples)
{
	glUniform1f(_mblur_pv.samples,samples);
}
