/* Copyright (c) 2016 Juan Wolfaardt

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

// The world-space vertex shader
#version 300 es

precision mediump int;
precision mediump float;

// Input view (camera) and last view matrices
uniform mat4 pview, lpview;

// Input vertex position and texture coord
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tc;

// Output texture coord for fragment shader
out vec2 vtc;
// Output view position and last view position for motion-blur shader
out vec4 vpos, vlpos;

void main(void) {
  // Set vertex position and texture coord
  vec4 p = vec4(pos, 1.0f);

  gl_Position = pview * p;
  vtc = tc;

  // Set the view and last view positions
  vpos = gl_Position;
  vlpos = lpview * p;
}
