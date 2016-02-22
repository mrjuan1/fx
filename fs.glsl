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

#version 300 es

precision mediump int;
precision mediump float;

uniform vec4 col;

uniform int texmode;
uniform sampler2D texmap;

in vec2 vtc;

layout(location=0) out vec4 oc;

void main(void)
{
	oc=col;

	switch(texmode)
	{
		case 1: oc*=texture(texmap,vtc);
	}

	if(oc.a==0.0) discard;
}
