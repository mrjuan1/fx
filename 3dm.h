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

#ifndef __3DM_H__
#define __3DM_H__

#ifdef __STDC__
	#include <stdlib.h>
	#define __USE_MISC
#endif /* __STDC__ */
#include <math.h>
#ifdef __STDC__
	float tanf(float), cosf(float), sinf(float);
#endif /* __STDC__ */

typedef float __attribute__((vector_size(4*sizeof(float)),aligned(sizeof(float)))) vec4;
typedef vec4 mat4[4];

float d2r(float a);

vec4 v3f(float a);
vec4 v0(void), v1(void);
vec4 v3(float x, float y, float z);
vec4 v4(float x, float y, float z, float w);

vec4 cross(vec4 a, vec4 b);
float dot(vec4 a, vec4 b);
vec4 unit(vec4 a);

void idmat(mat4 m);
void persp(mat4 m, float fov, float a, float n, float f);
void look(mat4 m, vec4 f, vec4 t);

void trans(mat4 m, vec4 p);
void rotx(mat4 m, float a);
void roty(mat4 m, float a);
void rotz(mat4 m, float a);
void scal(mat4 m, vec4 s);

void mult(mat4 m, const mat4 a, const mat4 b);
void invert(mat4 m, const mat4 a);

vec4 vmat(vec4 a, const mat4 b);

#endif /* __3DM_H__ */
