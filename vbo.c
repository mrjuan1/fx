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

#include "vbo.h"

const int _stride=5*sizeof(float);

byte _vbo_init=0;

unint _vbo=0;
char *_vdata=NULL;
int _vdata_size=0;

int get_stride(void)
{
	return _stride;
}

byte init_vbo(void)
{
	if(!_vbo_init && init_basic())
	{
		glGenBuffers(1,&_vbo);
		glBindBuffer(GL_ARRAY_BUFFER,_vbo);
		send_attribs();

		_vbo_init=1;
	}

	return _vbo_init;
}

void done_vbo(void)
{
	if(_vbo_init)
	{
		if(_vdata!=NULL)
		{
			free((void*)_vdata);
			_vdata=NULL;
			_vdata_size=0;
		}

		glDeleteBuffers(1,&_vbo);
		_vbo_init=0;
	}
}

void add_to_vbo(vbo_data *v, const void *data, int size)
{
	const int offset=_vdata_size;
	_vdata_size+=size;

	if(_vdata==NULL) _vdata=(char*)malloc(_vdata_size);
	else _vdata=(char*)realloc((void*)_vdata,_vdata_size);

	memcpy((void*)(_vdata+offset),data,size);

	v->offset=offset/_stride;
	v->count=size/_stride;
}

void update_vbo(void)
{
	if(_vdata!=NULL)
	{
		glBufferData(GL_ARRAY_BUFFER,_vdata_size,(const void*)_vdata,GL_STATIC_DRAW);

		free((void*)_vdata);
		_vdata=NULL;
		_vdata_size=0;
	}
	else info("No data in VBO to update\n");
}

void draw_vbo(const vbo_data *v)
{
	glDrawArrays(GL_TRIANGLES,v->offset,v->count);
}
