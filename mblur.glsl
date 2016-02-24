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

uniform sampler2D colmap;
uniform sampler2D velmap;

uniform float samples;

in vec2 vtc;

layout(location=0) out vec4 oc;

void main(void)
{
	vec3 vel=texture(velmap,vtc).rgb;

	oc=texture(colmap,vtc);
	if(vel.b==0.0f)
	{
		for(float i=1.0f ; i<samples ; i+=1.0f)
		{
			vec2 off=vel.xy*((i/samples)-0.5f);
			oc+=texture(colmap,vtc+off);
		}
		oc/=samples;
		oc.a=1.0f;
	}
}
