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

mat4 pview;

void init(void)
{
	if(init_quad())
	{
		mat4 proj, view;

		persp(proj,75.0f,asp(),3.0f,6.0f);
		look(view,v3(0.0f,-4.0f,2.0f),v0());
		mult(pview,proj,view);

		update_vbo();
	}
	else quit();
}

void loop(void)
{
	rotz(pview,20.0f*dt);
	send_pview(pview);

	clear();
	quad();

	if(kp(SDL_SCANCODE_DELETE) || kp(SDL_SCANCODE_ESCAPE))
		quit();
}

void done(void)
{
	done_quad();

	done_vbo();
	done_basic();
}
